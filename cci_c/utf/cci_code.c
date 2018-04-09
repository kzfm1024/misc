/**************************************************************/
/*    filename:cci_code.c コード生成とメモリ管理と解釈実行    */
/**************************************************************/
#define CCI_CODE_C
#include "cci.h"
#include "cci_prot.h"

#define CODE_SIZ 20000       /* コード格納領域サイズ              */
Inst code[CODE_SIZ+1];       /* コード格納                        */
int codeCt = -1;             /* code[0]〜code[codeCt]が有効       */

#define STACK_SIZ 100        /* スタックのサイズ                  */
#define STACK_BTM 0          /* スタックの底                      */
int stack[STACK_SIZ+1];      /* オペランドスタック                */
int stp = STACK_BTM;         /* stack[]のスタックポインタ         */

#define MEM_MAX 0xFFFF       /* メモリサイズ                      */
char memory[MEM_MAX+1];      /* memory[0]〜[MEM_MAX]がメモリ領域  */
int globalAdrs = 1*INTSIZE;  /* 先頭には静的領域サイズを入れる    */

int Pc;                      /* Pc:プログラムカウンタ */
int baseReg;                 /* ベースレジスタ        */

int gencode3(OpCode op, int fg, int dt) /* コード生成 */
{                                      /* memo:gencode1,gencode2は#define定義 */
  if (const_fold(op)) return codeCt;                              /* 定数畳込 */
  if (++codeCt > CODE_SIZ) {
    err_fi("コードが%dステップを超えた", CODE_SIZ); exit(1);
  }
  code[codeCt].opcode = op;                                           /* 命令 */
  code[codeCt].flag   = fg;                                         /* フラグ */
  code[codeCt].opdata = dt;                                           /* 数値 */

  return codeCt;                                      /* 格納した命令行を返す */
}

void gencode_Unary(TknKind kd) /* 単項命令作成 */
{
  OpCode op = NOP;

  switch (kd) {
  case Plus: return;                                   /* +のときは何もしない */
  case Minus: op = NEG; break;
  case Not:   op = NOT; break;
  case Incre: op = INC; break;
  case Decre: op = DEC; break;
  }
  gencode1(op);
}

void gencode_Binary(TknKind kd) /* 二項命令作成 */
{
  OpCode op = NOP;

  switch (kd) {
  case Plus:  op = ADD;  break;     case Minus:   op = SUB;  break;
  case Multi: op = MUL;  break;     case Divi:    op = DIV;  break;
  case Less:  op = LESS; break;     case LessEq:  op = LSEQ; break;
  case Great: op = GRT;  break;     case GreatEq: op = GTEQ; break;
  case Equal: op = EQU;  break;     case NotEq:   op = NTEQ; break;
  case And:   op = AND;  break;     case Or:      op = OR;   break;
  case Mod:   op = MOD;  break;
  }
  gencode1(op);
}

void to_leftVal(void) /* 左辺値にする */
{
  switch (code[codeCt].opcode) {
  case VAL: --codeCt; break;                                       /* VAL取消 */
  case LOD: code[codeCt].opcode = LDA; break;                   /* LOD → LDA */
  default:  err_s("不正な左辺値");                  /* a+b=100; や ++10; など */
  }
}

/* [式の値]を残すASSV命令を残さないASS命令に変更。またはDEL命令追加 */
void remove_val(void) /* 式の値の削除処理 */
{
  if (code[codeCt].opcode == ASSV) code[codeCt].opcode = ASS;        /* ASSに */
  else gencode1(DEL);                         /* あるいはDELで削除(var++など) */
}

int mallocS(char *s) /* 文字列確保 */
{
  int adrs;
  if (s == NULL) s = "";                                          /* 無難な値 */
  adrs = mallocG(strlen(s) + 1);                                  /* 領域確保 */
  strcpy(mem_adrs(adrs), s);                                  /* 文字列コピー */
  return adrs;
}

/* 大域領域にnバイトのメモリを確保し先頭位置(memory[]の添字)を返す */
int mallocG(int n) /* 大域メモリ割付 */
{
  if (n <= 0) return globalAdrs;                  /* 現在の空き領域先頭を返す */
  globalAdrs = (globalAdrs+3)/4*4;               /* 境界合わせのため4の倍数に */
  if (globalAdrs+n > MEM_MAX) {
    fprintf(stderr, "メモリ不足です。\n"); exit(1);
  }
  memset(memory+globalAdrs, 0, n);                              /* 領域クリア */
  globalAdrs += n;                                                /* 領域確保 */
  return globalAdrs - n;                                    /* 確保領域の先頭 */
}

char *mem_adrs(int indx) /* memory[]のアドレス */
{
  return memory + indx;
}

/* 定数取得。コードが1個だけ増え、それがLDI命令なら定数である */
/* varがNULLなら次番地を保存する（定数判定の前準備）          */
/* 定数なら真を返し、定数値を*varに入れ、コードは削除する     */
int get_const(int *var) /* 定数取得 */
{
  static int start_codeCt = -99;

  if (var == NULL) { start_codeCt = nextCodeCt(); return TRUE; }  /* 番地保存 */

  if (codeCt==start_codeCt && code[codeCt].opcode==LDI) {
    *var = code[codeCt].opdata;                                 /* 定数値格納 */
    --codeCt;                                                 /* コードは不要 */
    return TRUE;                                                /* 定数だった */
  }
  return FALSE;                                             /* 定数でなかった */
}

int const_fold(OpCode op) /* 定数畳込 */
{
  int n = codeCt, dt = code[n].opdata;

  if (n<=1 || code[n].opcode != LDI) return FALSE;                /* 圧縮なし */
  if (op == NOT) { code[n].opdata = !dt; return TRUE; }              /* !演算 */
  if (op == NEG) { code[n].opdata = -dt; return TRUE; }              /* -演算 */
  if (code[n-1].opcode==LDI && is_binaryOP(op)) {                 /* 二項演算 */
    code[n-1].opdata = binaryExpr(op, code[n-1].opdata, dt);
    --codeCt; return TRUE;                                       /* 1命令削除 */
  }
  return FALSE;                                                   /* 圧縮なし */
}

int is_binaryOP(OpCode op) /* 二項演算命令なら真 */
{
  return op==ADD  || op==SUB  || op==MUL || op==DIV  || op==MOD ||
         op==LESS || op==LSEQ || op==GRT || op==GTEQ || op==EQU ||
         op==NTEQ || op==AND  || op==OR;
}

int binaryExpr(OpCode op, int d1, int d2) /* 定数演算 */
{
  if ((op==DIV || op==MOD) && d2==0) { d2 = 1; err_s("ゼロ除算です。"); }
  switch(op) {
  case ADD:  d1 = d1 +  d2; break;
  case SUB:  d1 = d1 -  d2; break;
  case MUL:  d1 = d1 *  d2; break;
  case DIV:  d1 = d1 /  d2; break;
  case MOD:  d1 = d1 %  d2; break;
  case LESS: d1 = d1 <  d2; break;
  case LSEQ: d1 = d1 <= d2; break;
  case GRT:  d1 = d1 >  d2; break;
  case GTEQ: d1 = d1 >= d2; break;
  case EQU:  d1 = d1 == d2; break;
  case NTEQ: d1 = d1 != d2; break;
  case AND:  d1 = d1 && d2; break;
  case OR:   d1 = d1 || d2; break;
  }
  return d1;
}

int is_code(int n, OpCode op, int dt) /* コードが一致なら真 */
{
  return code[n].opcode==op && code[n].opdata==dt;
}

int nextCodeCt(void) /* 次の命令格納位置 */
{
  return codeCt+1;
}

void backPatch(int n, int adrs) /* コードの未定データを後埋 */
{
  code[n].opdata = adrs;
}

void backPatch_callAdrs(void) /* CALLの未定番地を確定 */
{
  int i, n;

  if (code[0].opdata < 0) err_s("main関数がない");       /* code[0]はCALL命令 */
  for (i=2; i<=codeCt; i++) {
    n = code[i].opdata;
    if (code[i].opcode==CALL && n<0) {          /* CALLの番地nが負(=未定)なら */
      code[i].opdata = tb_ptr(-n)->adrs;    /* 対応する関数記号表の番地を設定 */
      if (code[i].opdata < 0)                       /* それでも負値ならエラー */
        err_ss("未定義の関数", tb_ptr(-n)->name);
    }
  }
}

void backPatch_RET(int fncAdrs) /* RET関連JMPの未定番地処理 */
{
  int i;
  for(i=codeCt; i>=fncAdrs; i--)        /* 直前がRET関係JMPなら不要なので削除 */
    if (is_code(i, JMP, NO_FIX_RET_ADRS)) --codeCt; else break;
  for(i=codeCt; i>=fncAdrs; i--)                    /* 未定番地なら次番地設定 */
    if (is_code(i, JMP, NO_FIX_RET_ADRS)) code[i].opdata = codeCt+1;
}

void backPatch_BREAK(int looptop) /* break用JMPの飛先を後埋 */
{
  int i;
  for(i=codeCt; i>=looptop; i--)        /* 直前がbreak用JMPなら不要なので削除 */
    if (is_code(i, JMP, NO_FIX_BREAK_ADRS)) --codeCt; else break;
  for(i=codeCt; i>=looptop; i--)                    /* 未定番地なら次番地設定 */
    if (is_code(i, JMP, NO_FIX_BREAK_ADRS)) code[i].opdata = codeCt+1;
}

#define IntMem(n)      (*(int *)(memory+n))
#define ZERO_CHK()     if(stack[stp]==0)exe_err("ゼロ除算です。")
#define UNI_OP(op)     stack[stp] = op stack[stp]
#define BIN_OP(op)     stack[stp-1] = stack[stp-1] op stack[stp],--stp
#define INCDEC(dt)     IntMem(stack[stp])+=(dt),stack[stp]=IntMem(stack[stp])
#define ASSIGN(adr,dt) IntMem(adr)=dt
#define PUSH(dt)       stack[++stp]=dt
#define POP()          stack[stp--]

int execute(void) /* プログラム実行 */
{
  int opCode, opData, adrs;
  int start_localMEM = IntMem(0);        /* 0番地内容はスタック領域の開始位置 */

  Pc = 0;
  baseReg = MEM_MAX;
  stp = STACK_BTM;                               /* stack[]用ポインタの初期化 */

  for (;;) {
    if (Pc<0 || codeCt<Pc) {
        fprintf(stderr, "不正な終了(Pc=%d)\n", Pc); exit(1);      /* 念のため */
    }

    opCode = code[Pc].opcode;
    opData = code[Pc].opdata;
    adrs = (code[Pc].flag & 0x01) ? baseReg+opData : opData;      /* 絶対番地 */
    ++Pc;                                   /* Pcは次に実行する命令位置を示す */

    if (stp > STACK_SIZ) exe_err("stack overflow");
    if (stp < STACK_BTM) exe_err("stack underflow");

    switch(opCode) {
    case DEL:  --stp; break;                              /* スタックから削除 */
    case STOP: if (stp > 0) return POP(); else return 0;            /* OS戻値 */
    case JMP:  Pc = opData; break;                                /* ジャンプ */
    case JPT:  if (stack[stp--])  Pc = opData; break;         /* Trueジャンプ */
    case JPF:  if (!stack[stp--]) Pc = opData; break;        /* Falseジャンプ */
    case LIB:  if (opData == EXIT_F) return POP();           /* exit(n)の処理 */
               library(opData); break;            /* ライブラリ関数(関数種別) */
    case LOD:  PUSH(IntMem(adrs)); break;
    case LDA:  PUSH(adrs); break;
    case LDI:  PUSH(opData); break;
    case STO:  ASSIGN(adrs, stack[stp]); --stp; break;
    case ADBR: baseReg += opData;                             /* フレーム確保 */
               if (baseReg < start_localMEM) {
                 fprintf(stderr, "スタックメモリオーバー\n"); exit(1);
               }
               break;
    case NOP:  ++Pc; break;
    case ASS:  ASSIGN(stack[stp-1], stack[stp]); stp -= 2; break;
    case ASSV: ASSIGN(stack[stp-1], stack[stp]);
               stack[stp-1] = stack[stp]; --stp; break;
    case VAL:  stack[stp] = IntMem(stack[stp]);  break;           /* 値に変換 */
    case EQCMP: if (opData == stack[stp]) stack[stp] = 1; else PUSH(0);
               break;                                         /* switch用比較 */
    case CALL: PUSH(Pc); Pc = opData; break;      /* 戻番地を保存してジャンプ */
    case RET:  Pc = POP(); break;                             /* 戻番地の復活 */
    case INC:  INCDEC(+1); break;
    case DEC:  INCDEC(-1); break;
    case NOT:  UNI_OP(!);  break;
    case NEG:  UNI_OP(-);  break;
    case DIV:  ZERO_CHK(); BIN_OP(/); break;
    case MOD:  ZERO_CHK(); BIN_OP(%); break;
    case ADD:  BIN_OP(+);  break;
    case SUB:  BIN_OP(-);  break;
    case MUL:  BIN_OP(*);  break;
    case LESS: BIN_OP(<);  break;
    case LSEQ: BIN_OP(<=); break;
    case GRT:  BIN_OP(>);  break;
    case GTEQ: BIN_OP(>=); break;
    case EQU:  BIN_OP(==); break;
    case NTEQ: BIN_OP(!=); break;
    case AND:  BIN_OP(&&); break;
    case OR:   BIN_OP(||); break;
    default:   fprintf(stderr, "不正な命令(Pc=%d)\n", Pc-1); exit(1);
    }
  }
}

void library(int fnc_typ) /* 組込関数実行 */
{
  int dt = 0;
  char ss[40] = "";

  if (fnc_typ != INPUT_F) dt = stack[stp--];
  switch (fnc_typ) {
  case INPUT_F:   fgets(ss, 40, stdin); stack[++stp] = atoi(ss); break;
  case PRINTF1_F: printf("%s", mem_adrs(dt)); break;
  case PRINTF2_F: printf(mem_adrs(stack[stp--]), dt); break;
  }                      /* 注:exit(n)はexecute()内で処理 */
}

void code_dump(void) /* コードを表示 */
{
  int n;
  OpCode op;
  char nbrss[30] = "";

  for (n=0; n<=codeCt; n++) {
    op = code[n].opcode;
    printf("%04d: ", n);                                /* 生成した命令の番号 */

    sprintf(nbrss, "%d", code[n].opdata);
    if (code[n].flag && (op==LOD || op==LDA || op==STO)) strcat(nbrss, "[b]");

    switch (op) {
    case RET: case ASS:  case ASSV: case NOT:  case INC: case DEC:  case NEG:
    case ADD: case SUB:  case MUL:  case DIV:  case MOD: case LESS: case LSEQ:
    case GRT: case GTEQ: case EQU:  case NTEQ: case AND: case OR:   case VAL:
    case DEL: case NOP:  case STOP:
         printf("%-5s\n", ssOpCode[op]);
         if (op==RET || op==STOP) printf("\n");       /* 読みやすさのため改行 */
         break;
    case LOD:  case LDA: case LDI:  case STO:  case CALL: case ADBR: case LIB:
    case JMP:  case JPT: case JPF: case EQCMP:
         printf("%-5s %-11s\n", ssOpCode[op], nbrss);
         break;
    default:
         printf("Illegal code(#%d code=%d)\n", Pc-1, op);
         exit(1);
    }
  }
}

void exe_err(char *msg) /* 実行時エラー */
{
  fprintf(stderr, "Run-time error(#%d) %s\n", Pc-1, msg); exit(1);
}
