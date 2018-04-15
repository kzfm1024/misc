// token_p.c
/*--------------------------*/
/*    字句解析 token_p.c    */
/*--------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {                              /* トークンの種類等 */
    Lparen, Rparen, Plus,  Minus,   Multi,  Divi, Equal,  NotEq,
    Less,   LessEq, Great, GreatEq, SngQ,   DblQ, Assign, Semicolon,
    If,     Else,   Puts,  Ident,   IntNum,
    String, Letter, Digit, NulKind, EofTkn, Others, END_list
} Kind;

#define ID_SIZ 31                           /* 識別子長さ       */
#define TEXT_SIZ 100                        /* 文字列長さ       */
typedef struct {
    Kind kind;                              /* トークンの種類   */
    char text[TEXT_SIZ+1];                  /* トークン文字列   */
    int  intVal;                            /* 定数のときその値 */
} Token;

void initChTyp(void);
Token nextTkn(void);
int nextCh(void);
int is_ope2(int c1, int c2);
Token set_kind(Token t);
void err_exit(char *s);

Kind ctyp[256];                             /* 文字種表         */
Token token;                                /* トークン格納     */
FILE *fin;                                  /* ファイル処理     */

struct {                                    /* 字句と種別の対応 */
    char *ktext;                            /* 字句             */
    Kind kkind;                             /* 種別             */
} KeyWdTbl[] = {
    {"if" ,   If      }, {"else",  Else    },
    {"puts",  Puts    },
    {"(",     Lparen  }, {")",     Rparen  },
    {"+",     Plus    }, {"-",     Minus   },
    {"*",     Multi   }, {"/",     Divi    },
    {"==",    Equal   }, {"!=",    NotEq   },
    {"<",     Less    }, {"<=",    LessEq  },
    {">",     Great   }, {">=",    GreatEq },
    {"=",     Assign  }, {";",     Semicolon },
    {"",      END_list},
};

int main(int argc, char *argv[])
{
    if (argc == 1) exit(1);
    if ((fin=fopen(argv[1],"r")) == NULL) exit(1);

    printf("text      kind intVal\n");
    initChTyp();
    for (token = nextTkn(); token.kind != EofTkn; token = nextTkn()) {
        printf("%-10s %3d %d\n", token.text, token.kind, token.intVal);
    }
    return 0;
}

void initChTyp(void)                        /* 文字種表設定 */
{
  int i;

    for (i=0; i<256; i++)    { ctyp[i] = Others; }
    for (i='0'; i<='9'; i++) { ctyp[i] = Digit;  }
    for (i='A'; i<='Z'; i++) { ctyp[i] = Letter; }
    for (i='a'; i<='z'; i++) { ctyp[i] = Letter; }
    ctyp['_'] = Letter; ctyp['=']  = Assign;
    ctyp['('] = Lparen; ctyp[')']  = Rparen;
    ctyp['<'] = Less;   ctyp['>']  = Great;
    ctyp['+'] = Plus;   ctyp['-']  = Minus;
    ctyp['*'] = Multi;  ctyp['/']  = Divi;
    ctyp['\''] =SngQ;  ctyp['"']  = DblQ;
    ctyp[';'] = Semicolon;
}

Token nextTkn(void) /* トークン取得. 漢字やエスケープ文字は非対応 */
{
    Token  tkn = {NulKind, "", 0};
    int    ct, num, errF = 0;
    char   *p = tkn.text, *p_31 = p+ID_SIZ, *p_100 = p+TEXT_SIZ;
    static int ch = ' ';                    /* staticで前回文字を保持 */

    while (isspace(ch)) { ch = nextCh(); }  /* 空白読み捨て */
    if (ch == EOF) { tkn.kind = EofTkn; return tkn; }

    switch (ctyp[ch]) {
    case Letter:                            /* 識別子 */
        for ( ; ctyp[ch]==Letter || ctyp[ch]==Digit; ch=nextCh()) {
          if (p < p_31) *p++ = ch;          /* 識別子は最大31文字まで有効 */
        }
        *p = '\0';
        break;
    case Digit:                             /* 数字 */
        for (num=0; ctyp[ch]==Digit; ch=nextCh()) {
             num = num*10 + (ch-'0');
        }
        tkn.kind = IntNum;
        tkn.intVal = num;                   /* 値格納 */
        break;
    case SngQ:                              /* 文字定数 */
        ct = 0;
        for (ch=nextCh(); ch!=EOF && ch!='\n' && ch!='\''; ch=nextCh()) {
            if (++ct == 1) *p++ = tkn.intVal = ch; else errF = 1;
        }
        *p = '\0';
        if (ch == '\'') ch = nextCh(); else errF = 1;
        if (errF) err_exit("不正な文字定数");
        tkn.kind = IntNum;                  /* 整数定数として処理 */
        break;
    case DblQ:                              /* 文字列定数 */
        for (ch=nextCh(); ch!=EOF && ch!='\n' && ch!='"'; ch=nextCh()) {
            if (p >= p_100) errF = 1; else  *p++ = ch;
        }
        *p = '\0';
        if (errF) err_exit("文字列リテラルが長すぎる");
        if (ch != '"') err_exit("文字列リテラルが閉じていない");
        ch = nextCh();
        tkn.kind = String;
        break;
    default:                                /* 記号 */
        *p++ = ch; ch = nextCh();
        if (is_ope2(*(p-1), ch)) { *p++ = ch; ch = nextCh(); }
        *p = '\0';
    }
    if (tkn.kind == NulKind) tkn = set_kind(tkn);
    if (tkn.kind == Others) {
        printf("不正なトークンです(%s)\n", tkn.text); exit(1);
    }
    return tkn;
}

int nextCh(void)                            /* 次の1文字 */
{
    static int c = 0;
    if (c == EOF) return c;
    if ((c=fgetc(fin)) == EOF) fclose(fin);   /* 終了 */
    return c;
}

int is_ope2(int c1, int c2)                 /* 2文字演算子なら真 */
{
    char s[] = "    ";
    s[1] = c1; s[2] = c2;
    return strstr(" <= >= == != ", s) != NULL;
}

Token set_kind(Token t)
{
    int i;
    char *s = t.text;

    t.kind =  Others;
    for (i=0; KeyWdTbl[i].kkind != END_list; i++) {
        if (strcmp(s, KeyWdTbl[i].ktext)==0) {
            t.kind = KeyWdTbl[i].kkind; return t;
        }
    }
    if (ctyp[*s] == Letter)     t.kind = Ident;
    else if (ctyp[*s] == Digit) t.kind = IntNum;
    return t;
}

void err_exit(char *s)
{
    puts(s); exit(1);
}
