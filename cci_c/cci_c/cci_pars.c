/**********************************************************/
/*    filename:cci_pars.c 構文解析                        */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

Token token;                                          /* 現在処理中のトークン */
int blkNest = 0;                 /* ブロックの深さ 0:大域 1:関数 2:関数内の{} */
int err_ct = 0;                                               /* 総エラー個数 */
SymTbl tmpTb = {"",noId,NON_T,0,0,0,0};                     /* 一時格納記号表 */
SymTbl *fncPt = NULL;                             /* 翻訳中の関数の記号表位置 */
TknKind last_statement;                                 /* ブロック内最後の文 */

int localAdrs;                                        /* 局所変数アドレス管理 */
#define START_LocalAdrs 1*INTSIZE                     /* 局所変数割付開始番地 */
                                             /* 先頭4バイトは戻り番地用に確保 */
#define INT_P(p)   (int *)(p)                          /* int型ポインタに変換 */
/*---------------------- continue,break処理用*/
#define LOOP_SIZ 20
struct {
  TknKind stkind;                                                 /* 文の種類 */
  int looptop;                              /* ループ開始位置(continueの飛先) */
  int break_flg;                                   /* break文があれば真にする */
} loopNest[LOOP_SIZ+1];                               /* ループ文のネスト管理 */
int loopNest_ct = 0;                                      /* 使用中の最終位置 */
/*---------------------- switch処理用*/
#define SWTCH_SIZ 10
struct {
  int def_adrs;                                          /* defaultの対応番地 */
  int startCaseList;                                  /* caseList[]の開始位置 */
} swchNest[SWTCH_SIZ+1];                              /* switch文のネスト管理 */
int swchNest_ct = 0;                                      /* 使用中の最終位置 */

#define CASE_SIZ 100
struct {
  int value, adrs;                                      /* caseの値と対応番地 */
} caseList[CASE_SIZ+1];
int caseList_ct = 0;                                      /* 使用中の最終位置 */

int compile(char *fname) /* コンパイル */
{
  initChTyp();                                                    /* 文字種表 */
  gencode2(CALL, -1);                         /* main関数呼出。番地-1は仮設定 */
  gencode1(STOP);                                           /* プログラム終了 */

  fileOpen(fname);
  token = nextTkn();
  while (token.kind != EofTkn) {
    switch (token.kind) {
    case Int: case Void:                                        /* 変数か関数 */
      set_type(); set_name();                             /* 型名と名前を格納 */
      if (token.kind == '(') fncDecl(); else varDecl();      /* 関数,変数宣言 */
      break;
    case Semicolon:
      token = nextTkn();                                      /* 何もせず次へ */
      break;
    default:
      err_ss("構文エラー", token.text);
      token = nextTkn();
    }
  }
  if (err_ct == 0) backPatch_callAdrs();            /* 関数呼出未定番地を後埋 */
  *INT_P(mem_adrs(0)) = mallocG(0);            /* 静的領域サイズを0番地に格納 */

  if (err_ct > 0) fprintf(stderr, "%d個のエラーが発生しました。\n", err_ct);
  return err_ct == 0;                                     /* エラーなしなら真 */
}

void varDecl(void) /* 変数宣言 */
{
  for (;;) {
    set_aryLen();                                         /* 配列なら長さ設定 */
    enter(tmpTb, varId);                          /* 変数登録(アドレスも設定) */
    if (token.kind != ',') break;                                 /* 宣言終了 */
    token = nextTkn(); set_name();                              /* 次の変数名 */
  }
  token = chk_nextTkn(token, ';');                               /* ';'のはず */
}

void fncDecl(void) /* 関数宣言 */
{
  SymTbl *f1;

  localAdrs = START_LocalAdrs;                /* 局所領域割付けカウンタ初期化 */
  f1 = search_name(tmpTb.name);
  if (f1!=NULL && f1->nmKind!=fncId && f1->nmKind!=protId) {
    err_ss("識別子が重複している", f1->name); f1 = NULL;
  }

  fncPt = enter(tmpTb, fncId);                                /* 記号表に登録 */
  token = nextTkn();
  localTBL_open();                                      /* 局所用記号表を用意 */

  switch (token.kind) {                                           /* 引数処理 */
  case Void: token = nextTkn(); break;                         /* 引数 (void) */
  case ')':  break;                                            /* 引数なし () */
  default:                                                        /* 引数あり */
    for (;;) {
      set_type(); set_name(); enter(tmpTb, paraId);               /* 引数登録 */
      ++(fncPt->args);                                        /* 引数個数を+1 */
      if (token.kind != ',') break;                               /* 宣言終了 */
      token = nextTkn();                                          /* 次の引数 */
    }
  }
  token = chk_nextTkn(token, ')');                               /* ')'のはず */

  if (token.kind == ';') fncPt->nmKind = protId;          /* 関数プロトタイプ */
  set_adrs(fncPt);                          /* 関数入口番地と引数アドレス設定 */
  if (f1 != NULL) fncChk(f1, fncPt);                    /* 関数の正しさを確認 */

  switch (token.kind) {
  case ';': token = nextTkn(); break;                         /* プロトタイプ */
  case '{':
    if (IS_MAIN(fncPt)) set_main();                           /* main関数処理 */
    fncDecl_begin();                                          /* 関数入口処理 */
    block(1);                                                 /* 関数本体処理 */
    fncDecl_end();                                            /* 関数出口処理 */
    break;
  default:  err_s("関数の ; または { がない"); exit(1);
  }
  localTBL_close(fncPt);                                /* 局所用記号表を解消 */
  del_fncTable(f1, fncPt);                                /* 重複登録なら解消 */
  fncPt = NULL;                                               /* 関数処理終了 */
}

void fncChk(SymTbl *f1, SymTbl *f2) /* 関数重複チェック */
{
  if (f1->nmKind==fncId && f2->nmKind==fncId) {             /* 両方が関数本体 */
    err_ss("関数が再定義されている", f1->name); return;
  }
                                              /* 片方or両方が関数プロトタイプ */
  if (f1->dtTyp != f2->dtTyp || f1->args != f2->args) {
    err_ss("関数プロトタイプが不一致", f1->name); return;
  }
  return;
}

void fncDecl_begin(void) /* 関数入口処理 */
{
  int i;
  gencode2(ADBR, 0);             /* フレーム確保。サイズはfncDecl_end()で後埋 */
  gencode3(STO, LOCAL_F, 0);                                    /* 戻番地格納 */
  for (i=fncPt->args; i>=1; i--)
    gencode3(STO, LOCAL_F, (fncPt+i)->adrs);                    /* 実引数格納 */
}

void fncDecl_end(void) /* 関数出口処理 */
{
  backPatch(fncPt->adrs, -localAdrs);   /* フレームサイズ後埋(関数先頭はADBR) */
  if (last_statement != Return) {                   /* 関数末尾にreturnがない */
    if (IS_MAIN(fncPt)) gencode2(LDI, 0);             /* main関数なら戻値設定 */
    else if (fncPt->dtTyp != VOID_T)                    /* 本処理系の独自仕様 */
      err_s("本処理系では非void型関数の末尾には単独のreturn文が必要です。");
  }
  backPatch_RET(fncPt->adrs);           /* RET命令列へ飛ぶJMPの飛先番地を後埋 */
  gencode3(LOD, LOCAL_F, 0);                  /* 戻番地をオペランドスタックへ */
  gencode2(ADBR, localAdrs);                                /* フレームを解放 */
  gencode1(RET);                                                  /* 戻り命令 */
}

void set_main(void) /* main関数処理 */
{
  if (fncPt->dtTyp != INT_T || fncPt->args != 0)       /* 型と引数個数の確認 */
    err_s("main関数の書式が不正");
  backPatch(0, fncPt->adrs);                   /* 0番地のCALL命令の番地を後埋 */
}

void block(int is_func) /* {}内の処理 */
{
  TknKind kd = Others;

  token = nextTkn();
  ++blkNest;
  if (is_func) {                              /* 関数ブロックなら変数宣言処理 */
    while (token.kind == Int) { set_type(); set_name(); varDecl(); }
  }

  while (token.kind != '}') {                                     /* 文を処理 */
    kd = token.kind; statement();
  }
  last_statement = kd;                    /* 関数末尾のreturn有無確認に用いる */

  --blkNest;
  token = nextTkn();
}

/* この関数内のラベル処理(飛先処理)の明快さのために以下のマクロを使用する */
#define UPLABEL(pos)  pos=nextCodeCt()              /* 上ジャンプ用ラベル機能 */
#define JMP_UP(pos)   gencode2(JMP,pos)             /* JMPでUPLABELへ         */
#define JPT_UP(pos)   gencode2(JPT,pos)             /* JPTでUPLABELへ         */
#define JMP_DOWN(pos) pos=gencode2(JMP,0)           /* JMPでDWNLABELへ        */
#define JPF_DOWN(pos) pos=gencode2(JPF,0)           /* JPFでDWNLABELへ        */
#define DWNLABEL(pos) backPatch(pos,nextCodeCt())   /* 下ジャンプ用ラベル機能 */
void statement(void) /* 文の処理 */
{
  TknKind kd;
  SymTbl *tp;
  DtType ret_typ = fncPt->dtTyp;
  int i, val;
  int LB_TOP, LB_EXP2, LB_EXP3, LB_BODY, LB_ELSE, LB_END, LB_TBL;

  kd = token.kind;

  if (kd==While || kd==Do || kd==Switch) continue_break_begin(kd);
  switch (kd) {
  case Break:
    if (loopNest_ct == 0) err_s("不正なbreak");
    else {
      gencode2(JMP, NO_FIX_BREAK_ADRS);                     /* 該当末尾位置へ */
      loopNest[loopNest_ct].break_flg = TRUE;              /* breakありを記憶 */
    }
    token = chk_nextTkn(nextTkn(), ';');                          /* ; のはず */
    break;
  case Continue:
    gencode2(JMP, get_loopTop());                   /* ループ開始行へジャンプ */
    token = chk_nextTkn(nextTkn(), ';');                          /* ; のはず */
    break;
  case Case:
    token = nextTkn();
    get_const(NULL);                                          /* 定数判定準備 */
    expr_with_chk(0, ':');                                      /* [式]の処理 */
    if (!get_const(&val))      err_s("case式が定数式でない");
    else if (swchNest_ct == 0) err_s("対応するswitch文がない");
    else {
      for (i=swchNest[swchNest_ct].startCaseList; i<=caseList_ct; i++) {
        if (caseList[i].value == val) {
          err_s("case式の値が重複している"); break;
        }
      }
      incVar(&caseList_ct, CASE_SIZ, "case句が%d個を超えました。");
      caseList[caseList_ct].value = val;                        /* case値設定 */
      caseList[caseList_ct].adrs = nextCodeCt();              /* 対応番地設定 */
    }
    statement();                                                  /* 文の処理 */
    break;
  case Default:
    if (swchNest_ct == 0)
      err_s("対応するswitch文がない");
    else if (swchNest[swchNest_ct].def_adrs != -1)
      err_s("defaultが重複している");
    else
      swchNest[swchNest_ct].def_adrs = nextCodeCt();              /* 番地設定 */
    token = chk_nextTkn(nextTkn(), ':');                          /* : のはず */
    statement();                                                  /* 文の処理 */
    break;
  case For:
    /*[式1]*/
    token = chk_nextTkn(nextTkn(), '(');                          /* ( のはず */
    if (token.kind == ';')
      token = nextTkn();                                           /* 式1なし */
    else {
      expr_with_chk(0, ';');                                   /* [式1]の処理 */
      remove_val();                                               /* 式値不要 */
    }

    /*[式2]*/
    UPLABEL(LB_EXP2);                               /* ←┐                   */
    if (token.kind == ';') {                        /*   │式2なし            */
      gencode2(LDI, 1);                             /*   │真(1)にする        */
      token = nextTkn();                            /*   │                   */
    } else {                                        /*   │                   */
      expr_with_chk(0, ';');                        /*   │[式2]の処理        */
    }                                               /*   │                   */
    JPF_DOWN(LB_END);                               /* ─┼┐  false時        */
    JMP_DOWN(LB_BODY);                              /* ─┼┼┐true時         */
                                                    /*   │││               */
    /*[式3]*/                                       /*   │││               */
    continue_break_begin(kd);                       /*   │││               */
    UPLABEL(LB_EXP3);                               /* ←┼┼┼┐             */
    if (token.kind == ')')                          /*   ││││             */
      token = nextTkn();                            /*   ││││式3なし      */
    else {                                          /*   ││││             */
      expr_with_chk(0, ')');                        /*   ││││[式3]の処理  */
      remove_val();                                 /*   ││││式値不要     */
    }                                               /*   ││││             */
    JMP_UP(LB_EXP2);                                /* ─┘│││             */
                                                    /*     │││             */
    /*[本体]*/                                      /*     │││             */
    DWNLABEL(LB_BODY);                              /* ←─┼┘│             */
    statement();                                    /*     │  │[文]の処理   */
    JMP_UP(LB_EXP3);                                /* ──┼─┘繰り返し     */
                                                    /*     │                 */
    /*[末尾]*/                                      /*     │                 */
    DWNLABEL(LB_END);                               /* ←─┘終端             */
    break;
  case If:
    token = nextTkn();
    expr_with_chk('(', ')');                        /*       [式]の処理       */
    JPF_DOWN(LB_ELSE);                              /* ─┐  false時          */
    statement();                                    /*   │  [文1]の処理      */
    if (token.kind != Else) {                       /*   │                   */
      DWNLABEL(LB_ELSE);                            /* ←┘elseない時の終端   */
      break;                                        /*   │                   */
    }                                               /*   │elseある時         */
    JMP_DOWN(LB_END);                               /* ─┼┐                 */
    DWNLABEL(LB_ELSE);                              /* ←┘│                 */
    token = nextTkn();                              /*     │                 */
    statement();                                    /*     │[文2]の処理      */
    DWNLABEL(LB_END);                               /* ←─┘                 */
    break;
  case While:
    token = nextTkn();
    UPLABEL(LB_TOP);                                /* ←┐                   */
    expr_with_chk('(', ')');                        /*   │  [式]の処理       */
    JPF_DOWN(LB_END);                               /* ─┼┐false時          */
    statement();                                    /*   ││[文]の処理       */
    JMP_UP(LB_TOP);                                 /* ─┘│繰り返し         */
    DWNLABEL(LB_END);                               /* ←─┘                 */
    break;
  case Do:
    token = nextTkn();
    UPLABEL(LB_TOP);                                /* ←┐                   */
    statement();                                    /*   │[文]の処理         */
    if (token.kind == While) {                      /*   │                   */
      token = nextTkn();                            /*   │                   */
      expr_with_chk('(', ')');                      /*   │[式]の処理         */
      token = chk_nextTkn(token, ';');              /*   │; のはず           */
      JPT_UP(LB_TOP);                               /* ─┘true時             */
    } else {
      err_s("do終端のwhileがない");
    }
    break;
  case Switch:
    token = nextTkn();
    expr_with_chk('(', ')');                        /*      [式]の処理        */
    JMP_DOWN(LB_TBL);                               /* ─┐  テーブル処理へ   */
    swch_begin();                                   /*   │                   */
    statement();                                    /* case,default文の処理.  */
    JMP_DOWN(LB_END);                               /* ─┼┐末尾へ           */
    DWNLABEL(LB_TBL);                               /* ←┘│                 */
    swch_end();                                     /*     │テーブル処理     */
    DWNLABEL(LB_END);                               /* ←─┘                 */
    break;
  case Return:
    token = nextTkn();
    if (token.kind == ';') {                                      /* 戻値なし */
      if (ret_typ != VOID_T) err_s("return文に戻り値がない");
    } else {                                                      /* 戻値あり */
      expression();                                               /* 戻値作成 */
      if (ret_typ == VOID_T) err_s("void型関数に値を返すreturn文がある");
    }
    gencode2(JMP, NO_FIX_RET_ADRS);                         /* 関数出口処理へ */
    token = chk_nextTkn(token, ';');                              /* ; のはず */
    break;
  case Printf: case Exit:                                   /* void型組込関数 */
    sys_fncCall(kd);
    token = chk_nextTkn(token, ';');                  /* printf() + b; を防止 */
    break;
  case Input:                                             /* 非void型組込関数 */
    expr_with_chk(0, ';');                                    /* 通常の式解析 */
    remove_val();                                                 /* 式値不要 */
    break;
  case Incre: case Decre:                                      /* ++var --var */
    expr_with_chk(0, ';');
    remove_val();                                                 /* 式値不要 */
    break;
  case Ident:                                           /* 識別子(関数か変数) */
    tp = search(token.text);                                    /* 記号表位置 */
    if ((tp->nmKind==fncId || tp->nmKind==protId) && tp->dtTyp==VOID_T) {
      fncCall(tp);                                  /* void型関数はここで処理 */
      token = chk_nextTkn(token, ';');                            /* ; のはず */
    } else {
      expr_with_chk(0, ';');                                  /* 通常の式解析 */
      remove_val();                                               /* 式値不要 */
    }
    break;
  case Lbrace:                                                    /* 複合文{} */
    block(0);                                             /* 0:非関数ブロック */
    break;
  case Semicolon:                                 /* 空文,forの実行文等で出現 */
    token = nextTkn();
    break;
  case EofTkn:                             /* 最後の } を忘れた場合などに発生 */
    err_s("意図しない終了。'}'不足? "); exit(1);
  default:
    err_ss("不正な記述", token.text);
    token = nextTkn();
  }
  if (kd==For || kd==While || kd==Do || kd==Switch) continue_break_end();
}

void continue_break_begin(TknKind stmnt) /* CB開始処理 */
{
  incVar(&loopNest_ct, LOOP_SIZ, "制御文ネストが%d回を超えました。");
  loopNest[loopNest_ct].stkind = stmnt;                           /* 文の種類 */
  loopNest[loopNest_ct].looptop = nextCodeCt();                   /* 先頭番地 */
  loopNest[loopNest_ct].break_flg = FALSE;                 /* break文有無管理 */
}

void continue_break_end(void) /* CB終了処理 */
{
  if (loopNest[loopNest_ct].break_flg)                       /* breakがあれば */
    backPatch_BREAK(loopNest[loopNest_ct].looptop);             /* 飛先を後埋 */
  --loopNest_ct;                                        /* ネストをひとつ浅く */
}

int get_loopTop(void) /* 内側ループの先頭番地を返す */
{
  int i;
  for (i=loopNest_ct; i>0; i--) {
    if (loopNest[i].stkind != Switch) return loopNest[i].looptop;
  }
  err_s("continueがループ文内にない");
  return 0;
}

void swch_begin(void) /* switch文の開始 */
{
  incVar(&swchNest_ct, SWTCH_SIZ, "switchネストが%d回を超えました。");
  swchNest[swchNest_ct].def_adrs = -1;                   /* default番地未設定 */
  swchNest[swchNest_ct].startCaseList = caseList_ct+1;
}

void swch_end(void)     /* switch文の終了 */
{
  int i, start = swchNest[swchNest_ct].startCaseList;

  for (i=start; i<=caseList_ct; i++) {
    gencode2(EQCMP, caseList[i].value);                     /* caseの値と比較 */
    gencode2(JPT,  caseList[i].adrs);                       /* 対応ジャンプ先 */
  }
  gencode1(DEL);                                                /* 比較値削除 */
  if (swchNest[swchNest_ct].def_adrs != -1) {          /* defaultラベルがある */
    gencode2(JMP, swchNest[swchNest_ct].def_adrs);
  }
  caseList_ct = start - 1;                      /* caseListカウントを元に戻す */
  --swchNest_ct;                                        /* ネストをひとつ浅く */
}

/* 前後のトークンチェック付きでexpression()を実行。便利のために用意 */
void expr_with_chk(TknKind k1, TknKind k2) /* 式処理 */
{
  if (k1 != 0) token = chk_nextTkn(token, k1);
  expression();
  if (k2 != 0) token = chk_nextTkn(token, k2);
}

void expression(void) /* 式処理 */
{
  term(2);
  if (token.kind == '=') {
    to_leftVal();                                             /* 左辺値にする */
    token = nextTkn(); expression();
    gencode1(ASSV);                                     /* 代入する(式値設定) */
  }
}

void term(int n) /* 項関連処理。nは優先順位 */
{
  TknKind kd;

  if (n == 8) { factor(); return; }
  term(n+1);
  while (n == opOrder(token.kind)) {                /* 強さが同じ演算子が続く */
    kd = token.kind;
    token = nextTkn(); term(n+1);
    gencode_Binary(kd);                                         /* 二項演算子 */
  }
}

void factor(void) /* 因子関連処理 */
{
  SymTbl *tp;
  TknKind kd = token.kind;

  switch (kd) {
  case Plus: case Minus: case Not: case Incre: case Decre:     /* + - ! ++ -- */
    token = nextTkn(); factor();
    if (kd==Incre || kd==Decre) to_leftVal();                 /* 左辺値にする */
    gencode_Unary(kd);                                        /* 単項命令生成 */
    break;
  case IntNum:                                                    /* 整数定数 */
    gencode2(LDI, token.intVal);
    token = nextTkn();
    break;
  case Lparen:                                                      /* ( 式 ) */
    expr_with_chk('(', ')');
    break;
  case Printf: case Input: case Exit:
    if (kd != Input)
      err_ss("void型関数が式の中で使われている", token.text);
    sys_fncCall(kd);                                          /* 組込関数呼出 */
    break;
  case Ident:                                                       /* 識別子 */
    tp = search(token.text);                                    /* 記号表位置 */
    switch (tp->nmKind) {
    case fncId: case protId:                                      /* 関数呼出 */
      if (tp->dtTyp == VOID_T)
        err_ss("void型関数が式の中で使われている", tp->name);
      fncCall(tp);                                                /* 関数呼出 */
      break;
    case varId: case paraId:                                          /* 変数 */
      if (tp->aryLen == 0) {                                      /* 単純変数 */
        gencode3(LOD, b_flg(tp), tp->adrs);
        token = nextTkn();
      } else {                                                        /* 配列 */
        token = nextTkn();
        if (token.kind == '[') {                                  /* []がある */
          gencode3(LDA, b_flg(tp), tp->adrs);               /* 配列名アドレス */
          expr_with_chk('[', ']');                              /* 添字を取得 */
          gencode2(LDI, INTSIZE);                              /* 1要素サイズ */
          gencode1(MUL);                                 /* 添字×1要素サイズ */
          gencode1(ADD);                          /* それを配列アドレスに加算 */
          gencode1(VAL);                                          /* 内容取出 */
        } else {                                                  /* []がない */
          err_s("添字指定がない");                              /* 処理は続行 */
        }
      }
      if (token.kind==Incre || token.kind==Decre) {                 /* 後置++ */
        to_leftVal();                           /* 左辺値(アドレス表現)にする */
        if (token.kind==Incre) {gencode1(INC); gencode2(LDI,1); gencode1(SUB);}
        else                   {gencode1(DEC); gencode2(LDI,1); gencode1(ADD);}
        token = nextTkn();
      }
      break;
    }
    break;
  default:
    err_ss("不正な記述", token.text);
  }
}

void fncCall(SymTbl *fp) /* 関数呼出 */
{
  int  argCt = 0;

  token = chk_nextTkn(nextTkn(), '(');                            /* ( のはず */
  if (token.kind != ')') {                                      /* 引数がある */
    for (;;) {
      expression();                                           /* 引数式の処理 */
      ++argCt;                                                    /* 引数個数 */
      if (token.kind != ',') break;                       /* , なら引数が続く */
      token = nextTkn();
    }
  }
  token = chk_nextTkn(token, ')');                                /* ) のはず */

  if (argCt != fp->args)                                  /* 引数個数チェック */
    err_ss("関数の引数個数が不一致", fp->name);
  gencode2(CALL, fp->adrs);                                       /* 関数呼出 */
}

void sys_fncCall(TknKind kd) /* 組込関数呼出 */
{
  int fnc_typ = 0;
  char *form;

  token = chk_nextTkn(nextTkn(), '(');                            /* ( のはず */
  switch (kd) {
  case Exit: fnc_typ = EXIT_F; expression(); break;
  case Input: fnc_typ = INPUT_F; break;
  case Printf:
    if (token.kind != String)
      err_s("printfの第1引数が不正");                           /* 書式がない */
    gencode2(LDI, token.intVal);                        /* 文字列アドレス取得 */
    form = mem_adrs(token.intVal);                                    /* 書式 */
    token = nextTkn();
    if (token.kind != ',')
      fnc_typ = PRINTF1_F;                                   /* 引数1個タイプ */
    else {
      fnc_typ = PRINTF2_F;                                   /* 引数2個タイプ */
      token = nextTkn(); expression();                         /* 第2引数評価 */
      if (token.kind == ',')
        err_s("printfの引数は2個までです。");                   /* 引数が多い */
      if (!good_format(form))                         /* 安全のために書式確認 */
        err_s("printfの書式が不正");
    }
    break;
  }
  token = chk_nextTkn(token, ')');                                /* ) のはず */
  gencode2(LIB, fnc_typ);
}

void set_type(void) /* 型設定 */
{
  tmpTb.aryLen = tmpTb.adrs = tmpTb.args = 0;               /* クリアしておく */
  tmpTb.level = blkNest;                                     /* 0:大域 1:局所 */
  switch (token.kind) {
  case Int:  tmpTb.dtTyp = INT_T;  break;
  case Void: tmpTb.dtTyp = VOID_T; break;
  default:   err_ss("型指定誤り", token.text);
             tmpTb.dtTyp = INT_T;                              /* Intとみなす */
  }
  token = nextTkn();
}

void set_name(void) /* 名前設定 */
{
  if (token.kind == Ident) {
    tmpTb.name = s_malloc(token.text);                            /* 名前設定 */
    token = nextTkn();
  } else {
    err_ss("記述が不適切", token.text);
    tmpTb.name = "tmp$name1";                       /* ありえない名前を仮設定 */
  }
}

void set_aryLen(void) /* 配列サイズ設定 */
{
  tmpTb.aryLen = 0;
  if (token.kind != '[') return;                                /* 配列でない */

  token = nextTkn();
  if (token.kind == ']') {                                        /* []である */
    err_s("添字指定がない");
    token = nextTkn(); tmpTb.aryLen = 1; return;         /* 1だったことにする */
  }
  get_const(NULL);                                            /* 定数判定準備 */
  expr_with_chk(0, ']');                                        /* 添字を取得 */
  if (get_const(&(tmpTb.aryLen))) {                             /* 定数式なら */
    if (tmpTb.aryLen <= 0) {
      tmpTb.aryLen = 1; err_s("不正な添字");             /* 1だったことにする */
    }
  } else {
    err_s("配列幅指定が整数定数式でない");
  }

  if (token.kind == '[') {
    err_ss("多次元配列は宣言できない", token.text);
  }
}

void set_adrs(SymTbl *tp) /* 変数・引数の番地割当 */
{
  int i, size = INTSIZE;

  switch (tp->nmKind) {
  case varId:                                                        /*  変数 */
    if (tp->aryLen > 0) size *= tp->aryLen;             /* 配列ならサイズ計算 */
    if (is_global()) tp->adrs = mallocG(size);                    /* 大域変数 */
    else             tp->adrs = mallocL(size);                    /* 局所変数 */
    break;
  case fncId:                                                        /*  関数 */
    tp->adrs = nextCodeCt();                          /* 関数入口アドレス設定 */
    for (i=1; i<=tp->args; i++) {
      (tp+i)->adrs = mallocL(size);                     /* 引数のアドレス割当 */
    }
  }
}

int opOrder(TknKind kd) /* 二項演算子の優先順位 */
{
  switch (kd) {
  case Multi: case Divi:   case Mod:                 return 7;
  case Plus:  case Minus:                            return 6;
  case Less:  case LessEq: case Great: case GreatEq: return 5;
  case Equal: case NotEq:                            return 4;
  case And:                                          return 3;
  case Or:                                           return 2;
  case Assign:                                       return 1;
  default:                                           return 0;
  }
}

int is_global(void) /* グローバル領域なら真 */
{
  return blkNest == 0;
}

int mallocL(int size) /* 局所メモリ割付 */
{
  if (size < 0) size = 0;                                         /* 安全対策 */
  localAdrs += size;
  return localAdrs - size;                                    /* 割り付け番地 */
}

int good_format(char *form) /* 整数系書式確認 */
{
  char *p;

  while ((p=strstr(form, "%%")) != NULL)                        /* %%をマスク */
    *p = *(p+1) = '\1';
  if ((p=strchr(form, '%')) == NULL) return FALSE;                 /* %がない */
  ++p;
  if (*p == '-') ++p;                                           /* 左寄マーク */
  while (isdigit(*p)) { ++p; }                                      /* 幅指定 */
  if (*p == '\0') return FALSE;                             /* 変換文字がない */
  if (strchr("cdioxX", *p) == NULL) return FALSE;     /* 整数系変換文字がない */
  if (strchr(p, '%') != NULL) return FALSE;                /* %指定が複数ある */
  for (p=form; *p; p++) { if (*p == '\1') *p = '%'; }              /* %に戻す */
  return TRUE;                                                  /* 正しい書式 */
}
