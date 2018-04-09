/**********************************************************/
/*    filename:cci_tkn.c �g�[�N������                     */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

typedef struct {                                    /* ����Ǝ�ʂ̑Ή����Ǘ� */
  char *keyName;                                             /* void��for�Ȃ� */
  TknKind keyKind;                        /* �Ή�����l�BTknKind��cci.h�Œ�` */
} KeyWord;

KeyWord KeyWdTbl[] = {                          /* �\����L���Ǝ�ʂ̑Ή��\ */
  {"void"   , Void  }, {"int"     , Int    },
  {"if"     , If    }, {"else"    , Else   },
  {"for"    , For   }, {"while"   , While  },
  {"do"     , Do    }, {"switch"  , Switch },
  {"case"   , Case  }, {"default" , Default},
  {"break"  , Break }, {"continue", Continue },
  {"return" , Return}, {"printf"  , Printf },
  {"input"   , Input}, {"exit"    , Exit   },

  {"("  , Lparen    }, {")"  , Rparen    },
  {"{"  , Lbrace    }, {"}"  , Rbrace    },
  {"["  , Lbracket  }, {"]"  , Rbracket  },
  {"+"  , Plus      }, {"-"  , Minus     },
  {"*"  , Multi     }, {"/"  , Divi      },
  {"++" , Incre     }, {"--" , Decre     },
  {"==" , Equal     }, {"!=" , NotEq     },
  {"<"  , Less      }, {"<=" , LessEq    },
  {">"  , Great     }, {">=" , GreatEq   },
  {"&&" , And       }, {"||" , Or        },
  {"!"  , Not       }, {"%"  , Mod       },
  {":"  , Colon     }, {";"  , Semicolon },
  {"="  , Assign    }, {"#"  , Sharp     },
  {"\\" , Yen       }, {","  , Comma     },
  {"'"  , SngQ      }, {"\"" , DblQ      },
  {"$dummy", END_KeyList},
};

#define LIN_SIZ 250                             /* cci_tkn.c �\�[�X1�s�ő咷 */
FILE *fin;                                      /* �t�@�C������              */
int srcLineno = 0;                              /* �\�[�X�̍s�ԍ�            */
TknKind ctyp[256];                              /* ������\�̔z��            */

void initChTyp(void) /* ������\�̐ݒ� */
{
  int i;
  for (i=0; i<256; i++)    { ctyp[i] = Others; }
  for (i='0'; i<='9'; i++) { ctyp[i] = Digit;  }
  for (i='A'; i<='Z'; i++) { ctyp[i] = Letter; }
  for (i='a'; i<='z'; i++) { ctyp[i] = Letter; }
  ctyp['_']  = Letter;
  ctyp['(']  = Lparen;    ctyp[')']  = Rparen;
  ctyp['{']  = Lbrace;    ctyp['}']  = Rbrace;
  ctyp['[']  = Lbracket;  ctyp[']']  = Rbracket;
  ctyp['<']  = Less;      ctyp['>']  = Great;
  ctyp['+']  = Plus;      ctyp['-']  = Minus;
  ctyp['*']  = Multi;     ctyp['/']  = Divi;
  ctyp['!']  = Not;       ctyp['%']  = Mod;
  ctyp[':']  = Colon;     ctyp[';']  = Semicolon;
  ctyp['=']  = Assign;
  ctyp['\\'] = Yen;       ctyp[',']  = Comma;
  ctyp['\''] = SngQ;      ctyp['\"'] = DblQ;
}

void fileOpen(char *fname) /* �t�@�C�����J�� */
{
  if ((fin = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "%s���I�[�v���ł��܂���B\n", fname); exit(1);
  }
  srcLineno = 1;
}

#define P_SET() *p++=ch,ch=nextCh()
Token nextTkn(void) /* ���̃g�[�N�� */
{
  Token  tkn = {NulKind, "", 0};
  int    errF = FALSE, num, ct, n;
  char   *p = tkn.text, *p_end31 = p+ID_SIZ;
  char   lite[100+1], *lite_end = lite+100;
  static int ch = ' ';                        /* �O�񕶎���ێ����邽��static */

  while (isspace(ch)) { ch = nextCh(); }                      /* �󔒓ǂݎ̂� */
  if (ch == EOF) { tkn.kind = EofTkn; return tkn; }                    /* EOF */

  switch (ctyp[ch]) {
  case Letter:
    for ( ; ctyp[ch]==Letter || ctyp[ch]==Digit; ch = nextCh()) {
      if (p < p_end31) *p++ = ch;               /* ���ʎq�͍ő�31�����܂ŗL�� */
    }
    *p = '\0';
    break;
  case Digit:                                                     /* ���l�萔 */
    for (num=0; ctyp[ch]==Digit; ch = nextCh()) {
       num = num*10 + (ch-'0');
    }
    tkn.kind = IntNum;
    tkn.intVal = num;                                               /* �l�i�[ */
    sprintf(tkn.text, "%d", num);                             /* �G���[�\���p */
    break;
  case SngQ:                                                      /* �����萔 */
    for (ct=0,ch=nextCh(); ch!=EOF && ch!='\n' && ch!='\''; ch=nextCh()) {
      if (ch == '\\') { if ((ch=nextCh()) == 'n') ch = '\n'; }    /* \n�̏��� */
      if (++ct == 1) tkn.intVal = ch;                       /* �����萔�l�i�[ */
    }
    if (ct != 1) errF = TRUE;
    if (ch == '\'') ch = nextCh(); else errF = TRUE;
    if (errF) err_s("�s���ȕ����萔");
    tkn.kind = IntNum;                            /* �ȍ~�͐����萔�Ƃ��Ĉ��� */
    sprintf(tkn.text, "'%c'", tkn.intVal);                    /* �G���[�\���p */
    break;
  case DblQ:                                                    /* ������萔 */
    p = lite; ch = nextCh();
    while (ch!=EOF && ch!='\n' && ch!='"') {
      if (errF) { ch = nextCh(); continue; }
      if ((n=is_kanji(ch)) > 0) {
        while (n--) { if (p < lite_end) P_SET(); else errF = TRUE; }
        continue;
      }
      if (ch == '\\') { if ((ch=nextCh()) == 'n') ch = '\n'; }    /* \n�̏��� */
      if (p < lite_end) P_SET(); else errF = TRUE;
    }
    *p = '\0';
    if (errF) err_s("�����񃊃e��������������");
    if (ch == '"') ch = nextCh(); else err_s("�����񃊃e���������Ă��Ȃ�");
    tkn.kind = String;
    tkn.intVal = mallocS(lite);           /* ��������������Ɋm�ۂ��Ԓn���i�[ */

    tkn.text[0] = '\"';                         /*�ȉ��G���[�\���p�ɕ�����m��*/
    strncat(tkn.text+1, lite, 29);
    if (strlen(tkn.text) <= 29) strcat(tkn.text, "\"");
    break;
  default:                                                            /* �L�� */
    if (ch<0 || 127<ch) err_s("�s���ȕ������g���Ă���");
    if ((n=is_kanji(ch)) > 0) { while (n--) P_SET(); }
    else P_SET();
    if (is_ope2(*(p-1), ch)) P_SET();                              /* == �Ȃ� */
    *p = '\0';
  }
  if (tkn.kind == NulKind) tkn = set_kind(tkn);                   /* ��ʐݒ� */
  if (tkn.kind == Others) err_ss("�s���ȃg�[�N��", tkn.text);
  return tkn;
}

int nextCh(void) /* ����1���� */
{
  static int c = 1, quot_ch = 0;
  int c2;

  if (c == EOF) return c;
  if (c == '\n') ++srcLineno;                 /* �O��Ǎ������ōs�J�E���g�X�V */

  c = fgetc(fin);
  if (c == EOF) { fclose(fin); return c; }                            /* �I�� */
  if (quot_ch) {                                      /* ����(��)���e�������� */
    if (c==quot_ch || c=='\n') quot_ch = 0; /* ���e�����I���܂��͕��Ă��Ȃ� */
    return c;
  }
  if (c=='\'' || c=='"')
      quot_ch = c;                                    /* ����(��)���e�����J�n */
  else if (c == '/') {
    switch (c2 = fgetc(fin)) {
    case '/':                                             /* �s���܂ŃR�����g */
      while ((c=fgetc(fin))!=EOF && c!='\n')
        ;
      return c;                                                  /* EOF or \n */
    case '*':                                           /* �͈͌^�R�����g�J�n */
      for (c=0; (c2=fgetc(fin))!=EOF; c=c2) {
        if (c2 == '\n') ++srcLineno;                      /* �����͑��s�����Z */
        if (c=='*' && c2=='/') { c = ' '; return c; }         /* �R�����g�I�� */
      }
      err_s("/* �ɑΉ����� */ ���Ȃ�"); exit(1);
    }
    ungetc(c2, fin);                                                  /* �߂� */
  }
  else if (c == '*') {
    if ((c2=fgetc(fin)) == '/') {
      err_s("/* ���Ȃ��̂� */ �����o����"); exit(1);
    }
    ungetc(c2, fin);                                                  /* �߂� */
  }

  return c;
}

int is_ope2(int c1, int c2) /* 2�������Z�q�Ȃ�^ */
{
    char s[] = "    ";
    s[1] = c1; s[2] = c2;
    return strstr(" ++ -- <= >= == != && || ", s) != NULL;
}

Token set_kind(Token tk) /* �g�[�N����ʐݒ� */
{
  int i, ch = tk.text[0];

  tk.kind = Others;
  for (i=0; KeyWdTbl[i].keyKind != END_KeyList; i++) {
    if (strcmp(tk.text,KeyWdTbl[i].keyName)==0) {
      tk.kind = KeyWdTbl[i].keyKind; return tk;
    }
  }
  if (ctyp[ch] == Letter)     tk.kind = Ident;
  else if (ctyp[ch] == Digit) tk.kind = IntNum;
  return tk;
}

/* tk.kind==kd�Ȃ玟�̃g�[�N����Ԃ��B�قȂ�Ƃ��͂��̂܂�tk��Ԃ� */
Token chk_nextTkn(Token tk, TknKind kd) /* �m�F�t�g�[�N���擾 */
{
  char ss[100];
  if (tk.kind == kd) return nextTkn();
  else {
    sprintf(ss, "%s �̑O�� %c ������܂���", tk.text, kd); err_s(ss);
    return tk;                                                    /* �s��v�� */
  }
}

int get_lineNo(void) /* �Ǎ����̍s�ԍ� */
{
  return srcLineno;
}
