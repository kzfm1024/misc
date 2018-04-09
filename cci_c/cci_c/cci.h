/**********************************************************/
/*    filename:cci.h ���ʃw�b�_                           */
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* -------------------- define */
#define gencode1(op)     gencode3(op,0,0)
#define gencode2(op,dt)  gencode3(op,0,dt)

#define FALSE    0
#define TRUE     1
#define LOCAL_F  1
#define INTSIZE  sizeof(int)
#define NO_FIX_BREAK_ADRS -101  /* break�����p����Ԓn */
#define NO_FIX_RET_ADRS   -102  /* return�����p����Ԓn */
#define IS_MAIN(p) (strcmp((p)->name,"main")==0)

#define ID_SIZ   31             /* ���ʎq���� */
/* -------------------- enum struct etc */
typedef enum { NON_T, VOID_T, INT_T } DtType;     /* �^�� */
enum { EXIT_F=1, INPUT_F, PRINTF1_F, PRINTF2_F }; /* �g���֐���� */

typedef enum {                                                  /* ���߃R�[�h */
  NOP, INC, DEC, NEG, NOT, ADD, SUB, MUL, DIV, MOD, LESS, LSEQ,
  GRT, GTEQ,EQU, NTEQ,AND, OR,  CALL,DEL, JMP, JPT, JPF,  EQCMP,
  LOD, LDA, LDI, STO, ADBR,RET, ASS, ASSV,VAL, LIB, STOP
} OpCode;

#ifdef CCI_CODE_C
char *ssOpCode[] = {                                        /* �R�[�h�_���v�p */
   "NOP","INC","DEC","NEG","NOT","ADD","SUB","MUL","DIV","MOD",
   "LESS","LSEQ","GRT","GTEQ","EQU","NTEQ",
   "AND","OR","CALL","DEL","JMP","JPT","JPF","EQCMP","LOD","LDA","LDI","STO",
   "ADBR","RET","ASS","ASSV","VAL","LIB","STOP"
};
#endif

typedef enum {                                                /* �g�[�N���v�f */
  Lparen='(', Rparen=')',   Lbrace='{', Rbrace='}', Lbracket='[', Rbracket=']',
  Plus='+',   Minus='-',    Multi='*',  Divi='/',   Mod='%',      Not='!',
  Colon=':',  Semicolon=';',Assign='=', Sharp='#',  Yen='\\',     Comma=',',
  SngQ='\'',  DblQ='"',
  Void=150, Int,    If,       Else,    For,    While,   Do,     Switch, Case,
  Default,  Break,  Continue, Return,  Printf, Input,   Exit,   Incre,  Decre,
  Equal,    NotEq,  Less,     LessEq,  Great,  GreatEq, And,    Or,
  END_KeyList,
  Ident,    IntNum, String,   NulKind, Letter, Digit,   EofTkn, Others
} TknKind;

typedef struct {              /* �g�[�N���̊Ǘ� */
  TknKind kind;               /* �g�[�N���̎�� */
  char    text[ID_SIZ+1];     /* �g�[�N�������� */
  int     intVal;             /* �萔�̂Ƃ��̒l */
} Token;

typedef enum {                /* �L���\�o�^���̎�� */
  noId, varId, fncId, protId, paraId
} SymKind;

typedef struct {              /* �L���\�\��               */
  char    *name;              /* �ϐ���֐��̖��O         */
  SymKind nmKind;             /* ���                     */
  DtType  dtTyp;              /* �ϐ�,�֐��̌^            */
  int     aryLen;             /* �z�񒷁B0:�P���ϐ�       */
  char    level;              /* ��`���x�� 0:��� 1:�Ǐ� */
  char    args;               /* �֐��̏ꍇ�̈�����     */
  int     adrs;               /* �ϐ�,�֐��̔Ԓn          */
} SymTbl;

typedef struct {              /* ���ߌ�     */
  unsigned char opcode;       /* ���߃R�[�h */
  unsigned char flag;         /* �t���O     */
  int           opdata;       /* ���l���Ԓn */
} Inst;
