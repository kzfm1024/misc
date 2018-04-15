// minicalc.c
/*---------------------------------*/
/*    電卓プログラム minicalc.c    */
/*---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {                             /* トークンの種類 */
    Print, Lparen, Rparen, Plus, Minus, Multi, Divi,
    Assign, VarName, IntNum, EofTkn, Others
} Kind;

typedef struct {
    Kind kind;                             /* トークンの種類 */
    int  val;                              /* 整数値         */
} Token;

void input(void);
void statement(void);
void expression(void);
void term(void);
void factor(void);
Token nextTkn(void);
int nextCh(void);
void operate(Kind op);
void push(int n);
int pop(void);
void chkTkn(Kind kd);

#define STK_SIZ 20                         /* スタックサイズ */
int stack[STK_SIZ+1];                      /* スタック       */
int stkct;                                 /* スタック管理   */
Token token;                               /* トークン格納   */
char buf[80], *bufp;                       /* 入力用         */
int ch;                                    /* 取得文字を格納 */
int var[26];                               /* 変数a-z        */
int errF;                                  /* エラー発生     */

int main(void)
{
    while (1) {
        input();                           /* 入力   */
        token = nextTkn();                 /* 最初のトークン */
        if (token.kind == EofTkn) exit(1); /* 終了   */
        statement();                       /* 文実行 */
        if (errF) puts("  --err--");
    }
    return 0;
}

void input(void)
{
    errF = 0; stkct = 0;                   /* 初期設定           */
    buf[0] = '\0';
    fgets(buf, 80, stdin);                 /* 80文字以内の入力   */
    bufp = buf;                            /* 先頭文字に位置づけ */
    ch = nextCh();                         /* 最初の文字取得     */
}

void statement(void)                       /* 文 */
{
    int vNbr;

    switch (token.kind) {
    case VarName:                          /* 代入文     */
        vNbr = token.val;                  /* 代入先保存 */
        token = nextTkn();
        chkTkn(Assign); if (errF) break;   /* '=' のはず */
        token = nextTkn();
        expression();                      /* 右辺計算   */
        var[vNbr] = pop();                 /* 代入実行   */
        break;
    case Print:                            /* print文(?) */
        token = nextTkn();
        expression();
        chkTkn(EofTkn); if (errF) break;
        printf("  %d\n", pop());
        return;
    default:
        errF = 1;
    }
    chkTkn(EofTkn);                        /* 行末チェック */
}

void expression(void)                      /* 式 */
{
    Kind op;

    term();
    while (token.kind==Plus || token.kind==Minus) {
        op = token.kind;
        token = nextTkn(); term(); operate(op);
    }
}

void term(void)                            /* 項 */
{
    Kind op;

    factor();
    while (token.kind==Multi || token.kind==Divi) {
        op = token.kind;
        token = nextTkn(); factor(); operate(op);
    }
}

void factor(void)                          /* 因子     */
{
    switch (token.kind) {
    case VarName:                          /* 変数     */
        push(var[token.val]);
        break;
    case IntNum:                           /* 整数定数 */
        push(token.val);
        break;
    case Lparen:                           /* ( 式 )   */
        token = nextTkn();
        expression();
        chkTkn(Rparen);                    /* ) のはず */
        break;
    default:
        errF = 1;
    }
    token = nextTkn();
}

Token nextTkn(void)                        /* 次トークン */
{
    int num;
    Token tk = {Others, 0};

    while (isspace(ch))                    /* 空白読み捨て */
        ch = nextCh();
    if (isdigit(ch)) {                     /* 数字 */
        for (num=0; isdigit(ch); ch = nextCh())
           num = num*10 + (ch-'0');
        tk.val = num;                      /* 値格納 */
        tk.kind = IntNum;
    }
    else if (islower(ch)) {                /* 変数 */
        tk.val = ch - 'a';                 /* 変数番号0-25 */
        tk.kind = VarName;
        ch = nextCh();
    }
    else {
        switch (ch) {
        case '(':  tk.kind = Lparen; break;
        case ')':  tk.kind = Rparen; break;
        case '+':  tk.kind = Plus;   break;
        case '-':  tk.kind = Minus;  break;
        case '*':  tk.kind = Multi;  break;
        case '/':  tk.kind = Divi;   break;
        case '=':  tk.kind = Assign; break;
        case '?':  tk.kind = Print;  break;
        case '\0': tk.kind = EofTkn; break;
        }
        ch = nextCh();
    }
    return tk;
}

int nextCh(void)                           /* 次の1文字 */
{
    if (*bufp == '\0') return '\0'; else return *bufp++;
}

void operate(Kind op)                      /* 演算実行 */
{
    int d2 = pop(), d1 = pop();

    if (op==Divi && d2==0) { puts("  division by 0"); errF = 1; }
    if (errF) return;
    switch (op) {
    case Plus:  push(d1+d2); break;
    case Minus: push(d1-d2); break;
    case Multi: push(d1*d2); break;
    case Divi:  push(d1/d2); break;
    }
}

void push(int n)                           /* スタック積込 */
{
    if (errF) return;
    if (stkct+1 > STK_SIZ) { puts("stack overflow"); exit(1); }
    stack[++stkct] = n;
}

int pop(void)                              /* スタック取出 */
{
    if (errF) return 1;                    /* エラー時は単に1を返す */
    if (stkct < 1) { puts("stack underflow"); exit(1); }
    return stack[stkct--];
}

void chkTkn(Kind kd)                       /* トークン種別確認 */
{
    if (token.kind != kd) errF = 1;        /* 不一致 */
}
