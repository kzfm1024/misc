/**************************************************************/
/*    filename:cci_code.c �R�[�h�����ƃ������Ǘ��Ɖ��ߎ��s    */
/**************************************************************/
#define CCI_CODE_C
#include "cci.h"
#include "cci_prot.h"

#define CODE_SIZ 20000       /* �R�[�h�i�[�̈�T�C�Y              */
Inst code[CODE_SIZ+1];       /* �R�[�h�i�[                        */
int codeCt = -1;             /* code[0]�`code[codeCt]���L��       */

#define STACK_SIZ 100        /* �X�^�b�N�̃T�C�Y                  */
#define STACK_BTM 0          /* �X�^�b�N�̒�                      */
int stack[STACK_SIZ+1];      /* �I�y�����h�X�^�b�N                */
int stp = STACK_BTM;         /* stack[]�̃X�^�b�N�|�C���^         */

#define MEM_MAX 0xFFFF       /* �������T�C�Y                      */
char memory[MEM_MAX+1];      /* memory[0]�`[MEM_MAX]���������̈�  */
int globalAdrs = 1*INTSIZE;  /* �擪�ɂ͐ÓI�̈�T�C�Y������    */

int Pc;                      /* Pc:�v���O�����J�E���^ */
int baseReg;                 /* �x�[�X���W�X�^        */

int gencode3(OpCode op, int fg, int dt) /* �R�[�h���� */
{                                      /* memo:gencode1,gencode2��#define��` */
  if (const_fold(op)) return codeCt;                              /* �萔�� */
  if (++codeCt > CODE_SIZ) {
    err_fi("�R�[�h��%d�X�e�b�v�𒴂���", CODE_SIZ); exit(1);
  }
  code[codeCt].opcode = op;                                           /* ���� */
  code[codeCt].flag   = fg;                                         /* �t���O */
  code[codeCt].opdata = dt;                                           /* ���l */

  return codeCt;                                      /* �i�[�������ߍs��Ԃ� */
}

void gencode_Unary(TknKind kd) /* �P�����ߍ쐬 */
{
  OpCode op = NOP;

  switch (kd) {
  case Plus: return;                                   /* +�̂Ƃ��͉������Ȃ� */
  case Minus: op = NEG; break;
  case Not:   op = NOT; break;
  case Incre: op = INC; break;
  case Decre: op = DEC; break;
  }
  gencode1(op);
}

void gencode_Binary(TknKind kd) /* �񍀖��ߍ쐬 */
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

void to_leftVal(void) /* ���Ӓl�ɂ��� */
{
  switch (code[codeCt].opcode) {
  case VAL: --codeCt; break;                                       /* VAL��� */
  case LOD: code[codeCt].opcode = LDA; break;                   /* LOD �� LDA */
  default:  err_s("�s���ȍ��Ӓl");                  /* a+b=100; �� ++10; �Ȃ� */
  }
}

/* [���̒l]���c��ASSV���߂��c���Ȃ�ASS���߂ɕύX�B�܂���DEL���ߒǉ� */
void remove_val(void) /* ���̒l�̍폜���� */
{
  if (code[codeCt].opcode == ASSV) code[codeCt].opcode = ASS;        /* ASS�� */
  else gencode1(DEL);                         /* ���邢��DEL�ō폜(var++�Ȃ�) */
}

int mallocS(char *s) /* ������m�� */
{
  int adrs;
  if (s == NULL) s = "";                                          /* ����Ȓl */
  adrs = mallocG(strlen(s) + 1);                                  /* �̈�m�� */
  strcpy(mem_adrs(adrs), s);                                  /* ������R�s�[ */
  return adrs;
}

/* ���̈��n�o�C�g�̃��������m�ۂ��擪�ʒu(memory[]�̓Y��)��Ԃ� */
int mallocG(int n) /* ��惁�������t */
{
  if (n <= 0) return globalAdrs;                  /* ���݂̋󂫗̈�擪��Ԃ� */
  globalAdrs = (globalAdrs+3)/4*4;               /* ���E���킹�̂���4�̔{���� */
  if (globalAdrs+n > MEM_MAX) {
    fprintf(stderr, "�������s���ł��B\n"); exit(1);
  }
  memset(memory+globalAdrs, 0, n);                              /* �̈�N���A */
  globalAdrs += n;                                                /* �̈�m�� */
  return globalAdrs - n;                                    /* �m�ۗ̈�̐擪 */
}

char *mem_adrs(int indx) /* memory[]�̃A�h���X */
{
  return memory + indx;
}

/* �萔�擾�B�R�[�h��1���������A���ꂪLDI���߂Ȃ�萔�ł��� */
/* var��NULL�Ȃ玟�Ԓn��ۑ�����i�萔����̑O�����j          */
/* �萔�Ȃ�^��Ԃ��A�萔�l��*var�ɓ���A�R�[�h�͍폜����     */
int get_const(int *var) /* �萔�擾 */
{
  static int start_codeCt = -99;

  if (var == NULL) { start_codeCt = nextCodeCt(); return TRUE; }  /* �Ԓn�ۑ� */

  if (codeCt==start_codeCt && code[codeCt].opcode==LDI) {
    *var = code[codeCt].opdata;                                 /* �萔�l�i�[ */
    --codeCt;                                                 /* �R�[�h�͕s�v */
    return TRUE;                                                /* �萔������ */
  }
  return FALSE;                                             /* �萔�łȂ����� */
}

int const_fold(OpCode op) /* �萔�� */
{
  int n = codeCt, dt = code[n].opdata;

  if (n<=1 || code[n].opcode != LDI) return FALSE;                /* ���k�Ȃ� */
  if (op == NOT) { code[n].opdata = !dt; return TRUE; }              /* !���Z */
  if (op == NEG) { code[n].opdata = -dt; return TRUE; }              /* -���Z */
  if (code[n-1].opcode==LDI && is_binaryOP(op)) {                 /* �񍀉��Z */
    code[n-1].opdata = binaryExpr(op, code[n-1].opdata, dt);
    --codeCt; return TRUE;                                       /* 1���ߍ폜 */
  }
  return FALSE;                                                   /* ���k�Ȃ� */
}

int is_binaryOP(OpCode op) /* �񍀉��Z���߂Ȃ�^ */
{
  return op==ADD  || op==SUB  || op==MUL || op==DIV  || op==MOD ||
         op==LESS || op==LSEQ || op==GRT || op==GTEQ || op==EQU ||
         op==NTEQ || op==AND  || op==OR;
}

int binaryExpr(OpCode op, int d1, int d2) /* �萔���Z */
{
  if ((op==DIV || op==MOD) && d2==0) { d2 = 1; err_s("�[�����Z�ł��B"); }
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

int is_code(int n, OpCode op, int dt) /* �R�[�h����v�Ȃ�^ */
{
  return code[n].opcode==op && code[n].opdata==dt;
}

int nextCodeCt(void) /* ���̖��ߊi�[�ʒu */
{
  return codeCt+1;
}

void backPatch(int n, int adrs) /* �R�[�h�̖���f�[�^���㖄 */
{
  code[n].opdata = adrs;
}

void backPatch_callAdrs(void) /* CALL�̖���Ԓn���m�� */
{
  int i, n;

  if (code[0].opdata < 0) err_s("main�֐����Ȃ�");       /* code[0]��CALL���� */
  for (i=2; i<=codeCt; i++) {
    n = code[i].opdata;
    if (code[i].opcode==CALL && n<0) {          /* CALL�̔Ԓnn����(=����)�Ȃ� */
      code[i].opdata = tb_ptr(-n)->adrs;    /* �Ή�����֐��L���\�̔Ԓn��ݒ� */
      if (code[i].opdata < 0)                       /* ����ł����l�Ȃ�G���[ */
        err_ss("����`�̊֐�", tb_ptr(-n)->name);
    }
  }
}

void backPatch_RET(int fncAdrs) /* RET�֘AJMP�̖���Ԓn���� */
{
  int i;
  for(i=codeCt; i>=fncAdrs; i--)        /* ���O��RET�֌WJMP�Ȃ�s�v�Ȃ̂ō폜 */
    if (is_code(i, JMP, NO_FIX_RET_ADRS)) --codeCt; else break;
  for(i=codeCt; i>=fncAdrs; i--)                    /* ����Ԓn�Ȃ玟�Ԓn�ݒ� */
    if (is_code(i, JMP, NO_FIX_RET_ADRS)) code[i].opdata = codeCt+1;
}

void backPatch_BREAK(int looptop) /* break�pJMP�̔����㖄 */
{
  int i;
  for(i=codeCt; i>=looptop; i--)        /* ���O��break�pJMP�Ȃ�s�v�Ȃ̂ō폜 */
    if (is_code(i, JMP, NO_FIX_BREAK_ADRS)) --codeCt; else break;
  for(i=codeCt; i>=looptop; i--)                    /* ����Ԓn�Ȃ玟�Ԓn�ݒ� */
    if (is_code(i, JMP, NO_FIX_BREAK_ADRS)) code[i].opdata = codeCt+1;
}

#define IntMem(n)      (*(int *)(memory+n))
#define ZERO_CHK()     if(stack[stp]==0)exe_err("�[�����Z�ł��B")
#define UNI_OP(op)     stack[stp] = op stack[stp]
#define BIN_OP(op)     stack[stp-1] = stack[stp-1] op stack[stp],--stp
#define INCDEC(dt)     IntMem(stack[stp])+=(dt),stack[stp]=IntMem(stack[stp])
#define ASSIGN(adr,dt) IntMem(adr)=dt
#define PUSH(dt)       stack[++stp]=dt
#define POP()          stack[stp--]

int execute(void) /* �v���O�������s */
{
  int opCode, opData, adrs;
  int start_localMEM = IntMem(0);        /* 0�Ԓn���e�̓X�^�b�N�̈�̊J�n�ʒu */

  Pc = 0;
  baseReg = MEM_MAX;
  stp = STACK_BTM;                               /* stack[]�p�|�C���^�̏����� */

  for (;;) {
    if (Pc<0 || codeCt<Pc) {
        fprintf(stderr, "�s���ȏI��(Pc=%d)\n", Pc); exit(1);      /* �O�̂��� */
    }

    opCode = code[Pc].opcode;
    opData = code[Pc].opdata;
    adrs = (code[Pc].flag & 0x01) ? baseReg+opData : opData;      /* ��ΔԒn */
    ++Pc;                                   /* Pc�͎��Ɏ��s���閽�߈ʒu������ */

    if (stp > STACK_SIZ) exe_err("stack overflow");
    if (stp < STACK_BTM) exe_err("stack underflow");

    switch(opCode) {
    case DEL:  --stp; break;                              /* �X�^�b�N����폜 */
    case STOP: if (stp > 0) return POP(); else return 0;            /* OS�ߒl */
    case JMP:  Pc = opData; break;                                /* �W�����v */
    case JPT:  if (stack[stp--])  Pc = opData; break;         /* True�W�����v */
    case JPF:  if (!stack[stp--]) Pc = opData; break;        /* False�W�����v */
    case LIB:  if (opData == EXIT_F) return POP();           /* exit(n)�̏��� */
               library(opData); break;            /* ���C�u�����֐�(�֐����) */
    case LOD:  PUSH(IntMem(adrs)); break;
    case LDA:  PUSH(adrs); break;
    case LDI:  PUSH(opData); break;
    case STO:  ASSIGN(adrs, stack[stp]); --stp; break;
    case ADBR: baseReg += opData;                             /* �t���[���m�� */
               if (baseReg < start_localMEM) {
                 fprintf(stderr, "�X�^�b�N�������I�[�o�[\n"); exit(1);
               }
               break;
    case NOP:  ++Pc; break;
    case ASS:  ASSIGN(stack[stp-1], stack[stp]); stp -= 2; break;
    case ASSV: ASSIGN(stack[stp-1], stack[stp]);
               stack[stp-1] = stack[stp]; --stp; break;
    case VAL:  stack[stp] = IntMem(stack[stp]);  break;           /* �l�ɕϊ� */
    case EQCMP: if (opData == stack[stp]) stack[stp] = 1; else PUSH(0);
               break;                                         /* switch�p��r */
    case CALL: PUSH(Pc); Pc = opData; break;      /* �ߔԒn��ۑ����ăW�����v */
    case RET:  Pc = POP(); break;                             /* �ߔԒn�̕��� */
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
    default:   fprintf(stderr, "�s���Ȗ���(Pc=%d)\n", Pc-1); exit(1);
    }
  }
}

void library(int fnc_typ) /* �g���֐����s */
{
  int dt = 0;
  char ss[40] = "";

  if (fnc_typ != INPUT_F) dt = stack[stp--];
  switch (fnc_typ) {
  case INPUT_F:   fgets(ss, 40, stdin); stack[++stp] = atoi(ss); break;
  case PRINTF1_F: printf("%s", mem_adrs(dt)); break;
  case PRINTF2_F: printf(mem_adrs(stack[stp--]), dt); break;
  }                      /* ��:exit(n)��execute()���ŏ��� */
}

void code_dump(void) /* �R�[�h��\�� */
{
  int n;
  OpCode op;
  char nbrss[30] = "";

  for (n=0; n<=codeCt; n++) {
    op = code[n].opcode;
    printf("%04d: ", n);                                /* �����������߂̔ԍ� */

    sprintf(nbrss, "%d", code[n].opdata);
    if (code[n].flag && (op==LOD || op==LDA || op==STO)) strcat(nbrss, "[b]");

    switch (op) {
    case RET: case ASS:  case ASSV: case NOT:  case INC: case DEC:  case NEG:
    case ADD: case SUB:  case MUL:  case DIV:  case MOD: case LESS: case LSEQ:
    case GRT: case GTEQ: case EQU:  case NTEQ: case AND: case OR:   case VAL:
    case DEL: case NOP:  case STOP:
         printf("%-5s\n", ssOpCode[op]);
         if (op==RET || op==STOP) printf("\n");       /* �ǂ݂₷���̂��߉��s */
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

void exe_err(char *msg) /* ���s���G���[ */
{
  fprintf(stderr, "Run-time error(#%d) %s\n", Pc-1, msg); exit(1);
}
