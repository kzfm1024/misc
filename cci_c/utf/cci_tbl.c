/**********************************************************/
/*    filename:cci_tbl.c 記号表処理                       */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

/* ----------------------------テーブル */
#define TBL_MAX    1000      /* 記号表のサイズ   */
#define start_GTBL 1         /* 大域用の開始位置 */
#define LTBL_EMPTY 9999      /* 局所記号表空き   */
SymTbl table[TBL_MAX+1];     /* 記号表           */
int start_LTBL = LTBL_EMPTY; /* 局所用の開始位置 */
int tableCt = 0;             /* 使用中の最終位置 */

SymTbl *enter(SymTbl tb, SymKind kind) /* 記号表登録 */
{
  int tbNo;

  if ((kind==varId || kind==paraId) && tb.dtTyp==VOID_T) {
    err_s("変数型誤り(void)");
    tb.dtTyp = INT_T;                                            /* intにする */
  }
  tb.nmKind = kind;
  nameChk(tb);                                       /* 引数と変数時の重複chk */

  if (tableCt >= TBL_MAX) { err_s("記号表 over"); exit(1); }
  tbNo = ++tableCt;                                             /* 記号表番号 */
  table[tbNo] = tb;                                           /* 記号表に登録 */

  if (kind == paraId) ++table[tbNo].level;  /* 引数を関数ブロックに入れる調整 */
  if (kind == varId) set_adrs(&table[tbNo]);          /* 変数ならアドレス設定 */
  if (kind == fncId) table[tbNo].adrs = -tbNo;    /* 記号表番号の負値を関数の */
                                          /* 番地として仮設定(番地未決定の意) */
  return &table[tbNo];                                /* 記号表ポインタを返す */
}

void localTBL_open(void) /* 局所記号表開始 */
{
  start_LTBL = tableCt + 1;                     /* 局所用記号表開始位置設定 */
}

void localTBL_close(SymTbl *fp) /* 局所記号表終了 */
{                         /* ↓引数登録を大域領域に残すため引数個数分だけ調整 */
  tableCt = start_LTBL - 1 + fp->args;                  /* 大域用最終位置回復 */
  start_LTBL = LTBL_EMPTY;                                /* 局所記号表クリア */
}

SymTbl *search(char *s) /* 記号表検索(サービスあり) */
{
  SymTbl *p;
  static SymTbl *dmy_p=NULL, wkTb = {"tmp$name2",varId,INT_T,0,0,0,0};

  p = search_name(s);
  if (p == NULL) {                                    /* なければ仮変数を返す */
    err_ss("未定義の識別子", s);
    if (dmy_p == NULL) dmy_p = enter(wkTb, varId);              /* 仮変数登録 */
    p = dmy_p;
  }
  return p;
}

SymTbl *search_name(char *s) /* 記号表検索 */
{
  int i;                         /* 注:局所記号表がないときはstart_LTBLは9999 */
  for (i=tableCt; i>=start_LTBL; i--) {                 /* 先に局所領域を探す */
    if (strcmp(table[i].name,s)==0) return table + i;
  }
  for (; i>=start_GTBL; i--) {                          /* 次に大域領域を探す */
    if (table[i].nmKind!=paraId && strcmp(table[i].name,s)==0)
      return table + i;
  }
  return NULL;                                                    /* なかった */
}

void nameChk(SymTbl tb) /* 引数と変数の名前重複チェック */
{
  SymTbl *p;
  extern int blkNest;                                         /* (cci_pars.c) */
  int nest = blkNest;

  if (tb.nmKind!=paraId && tb.nmKind!=varId) return;
  if ((p = search_name(tb.name)) == NULL) return;
  if (tb.nmKind == paraId) ++nest;                  /* 引数検査用に記憶域調整 */
  if (p->level >= nest) err_ss("識別子が重複している", tb.name);
}

void del_fncTable(SymTbl *f1, SymTbl *f2) /* 重複登録の解消 */
{
  int i;
  if (f1 == NULL) return;
  if (f1->dtTyp!=f2->dtTyp || f1->args!=f2->args)  return;
  if (f1->nmKind==protId && f2->nmKind==fncId) {  /* f1がプロト、f2が本体なら */
    for (i=0; i<=f2->args; i++) { *(f1+i) = *(f2+i); }      /* f2をf1にコピー */
  }
  tableCt -= (f2->args + 1);                              /* 新しい登録を取消 */
  return;
}

int b_flg(SymTbl *tp) /* 局所変数なら1を返す */
{
  if (tp->level == 0) return 0; else return 1;                   /* 大域なら0 */
}

SymTbl *tb_ptr(int n) /* 記号表アドレスを返す */
{
  return &table[n];
}
