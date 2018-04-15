/**********************************************************/
/*    filename:cci_tkn.c トークン処理                     */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

typedef struct {                                    /* 字句と種別の対応を管理 */
  char *keyName;                                             /* voidやforなど */
  TknKind keyKind;                        /* 対応する値。TknKindはcci.hで定義 */
} KeyWord;

KeyWord KeyWdTbl[] = {                          /* 予約語や記号と種別の対応表 */
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

#define LIN_SIZ 250                             /* cci_tkn.c ソース1行最大長 */
FILE *fin;                                      /* ファイル処理              */
int srcLineno = 0;                              /* ソースの行番号            */
TknKind ctyp[256];                              /* 文字種表の配列            */

void initChTyp(void) /* 文字種表の設定 */
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

void fileOpen(char *fname) /* ファイルを開く */
{
  if ((fin = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "%sをオープンできません。\n", fname); exit(1);
  }
  srcLineno = 1;
}

#define P_SET() *p++=ch,ch=nextCh()
Token nextTkn(void) /* 次のトークン */
{
  Token  tkn = {NulKind, "", 0};
  int    errF = FALSE, num, ct, n;
  char   *p = tkn.text, *p_end31 = p+ID_SIZ;
  char   lite[100+1], *lite_end = lite+100;
  static int ch = ' ';                        /* 前回文字を保持するためstatic */

  while (isspace(ch)) { ch = nextCh(); }                      /* 空白読み捨て */
  if (ch == EOF) { tkn.kind = EofTkn; return tkn; }                    /* EOF */

  switch (ctyp[ch]) {
  case Letter:
    for ( ; ctyp[ch]==Letter || ctyp[ch]==Digit; ch = nextCh()) {
      if (p < p_end31) *p++ = ch;               /* 識別子は最大31文字まで有効 */
    }
    *p = '\0';
    break;
  case Digit:                                                     /* 数値定数 */
    for (num=0; ctyp[ch]==Digit; ch = nextCh()) {
       num = num*10 + (ch-'0');
    }
    tkn.kind = IntNum;
    tkn.intVal = num;                                               /* 値格納 */
    sprintf(tkn.text, "%d", num);                             /* エラー表示用 */
    break;
  case SngQ:                                                      /* 文字定数 */
    for (ct=0,ch=nextCh(); ch!=EOF && ch!='\n' && ch!='\''; ch=nextCh()) {
      if (ch == '\\') { if ((ch=nextCh()) == 'n') ch = '\n'; }    /* \nの処理 */
      if (++ct == 1) tkn.intVal = ch;                       /* 文字定数値格納 */
    }
    if (ct != 1) errF = TRUE;
    if (ch == '\'') ch = nextCh(); else errF = TRUE;
    if (errF) err_s("不正な文字定数");
    tkn.kind = IntNum;                            /* 以降は整数定数として扱う */
    sprintf(tkn.text, "'%c'", tkn.intVal);                    /* エラー表示用 */
    break;
  case DblQ:                                                    /* 文字列定数 */
    p = lite; ch = nextCh();
    while (ch!=EOF && ch!='\n' && ch!='"') {
      if (errF) { ch = nextCh(); continue; }
      if ((n=is_kanji(ch)) > 0) {
        while (n--) { if (p < lite_end) P_SET(); else errF = TRUE; }
        continue;
      }
      if (ch == '\\') { if ((ch=nextCh()) == 'n') ch = '\n'; }    /* \nの処理 */
      if (p < lite_end) P_SET(); else errF = TRUE;
    }
    *p = '\0';
    if (errF) err_s("文字列リテラルが長すぎる");
    if (ch == '"') ch = nextCh(); else err_s("文字列リテラルが閉じていない");
    tkn.kind = String;
    tkn.intVal = mallocS(lite);           /* 文字列をメモリに確保し番地を格納 */

    tkn.text[0] = '\"';                         /*以下エラー表示用に文字列確保*/
    strncat(tkn.text+1, lite, 29);
    if (strlen(tkn.text) <= 29) strcat(tkn.text, "\"");
    break;
  default:                                                            /* 記号 */
    if (ch<0 || 127<ch) err_s("不正な文字が使われている");
    if ((n=is_kanji(ch)) > 0) { while (n--) P_SET(); }
    else P_SET();
    if (is_ope2(*(p-1), ch)) P_SET();                              /* == など */
    *p = '\0';
  }
  if (tkn.kind == NulKind) tkn = set_kind(tkn);                   /* 種別設定 */
  if (tkn.kind == Others) err_ss("不正なトークン", tkn.text);
  return tkn;
}

int nextCh(void) /* 次の1文字 */
{
  static int c = 1, quot_ch = 0;
  int c2;

  if (c == EOF) return c;
  if (c == '\n') ++srcLineno;                 /* 前回読込文字で行カウント更新 */

  c = fgetc(fin);
  if (c == EOF) { fclose(fin); return c; }                            /* 終了 */
  if (quot_ch) {                                      /* 文字(列)リテラル内部 */
    if (c==quot_ch || c=='\n') quot_ch = 0; /* リテラル終了または閉じていない */
    return c;
  }
  if (c=='\'' || c=='"')
      quot_ch = c;                                    /* 文字(列)リテラル開始 */
  else if (c == '/') {
    switch (c2 = fgetc(fin)) {
    case '/':                                             /* 行末までコメント */
      while ((c=fgetc(fin))!=EOF && c!='\n')
        ;
      return c;                                                  /* EOF or \n */
    case '*':                                           /* 範囲型コメント開始 */
      for (c=0; (c2=fgetc(fin))!=EOF; c=c2) {
        if (c2 == '\n') ++srcLineno;                      /* ここは即行数加算 */
        if (c=='*' && c2=='/') { c = ' '; return c; }         /* コメント終了 */
      }
      err_s("/* に対応する */ がない"); exit(1);
    }
    ungetc(c2, fin);                                                  /* 戻す */
  }
  else if (c == '*') {
    if ((c2=fgetc(fin)) == '/') {
      err_s("/* がないのに */ を検出した"); exit(1);
    }
    ungetc(c2, fin);                                                  /* 戻す */
  }

  return c;
}

int is_ope2(int c1, int c2) /* 2文字演算子なら真 */
{
    char s[] = "    ";
    s[1] = c1; s[2] = c2;
    return strstr(" ++ -- <= >= == != && || ", s) != NULL;
}

Token set_kind(Token tk) /* トークン種別設定 */
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

/* tk.kind==kdなら次のトークンを返す。異なるときはそのままtkを返す */
Token chk_nextTkn(Token tk, TknKind kd) /* 確認付トークン取得 */
{
  char ss[100];
  if (tk.kind == kd) return nextTkn();
  else {
    sprintf(ss, "%s の前に %c がありません", tk.text, kd); err_s(ss);
    return tk;                                                    /* 不一致時 */
  }
}

int get_lineNo(void) /* 読込中の行番号 */
{
  return srcLineno;
}
