// minicalc.c
/*---------------------------------*/
/*    �d��v���O���� minicalc.c    */
/*---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {                             /* �g�[�N���̎�� */
    Print, Lparen, Rparen, Plus, Minus, Multi, Divi,
    Assign, VarName, IntNum, EofTkn, Others
} Kind;

typedef struct {
    Kind kind;                             /* �g�[�N���̎�� */
    int  val;                              /* �����l         */
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

#define STK_SIZ 20                         /* �X�^�b�N�T�C�Y */
int stack[STK_SIZ+1];                      /* �X�^�b�N       */
int stkct;                                 /* �X�^�b�N�Ǘ�   */
Token token;                               /* �g�[�N���i�[   */
char buf[80], *bufp;                       /* ���͗p         */
int ch;                                    /* �擾�������i�[ */
int var[26];                               /* �ϐ�a-z        */
int errF;                                  /* �G���[����     */

int main(void)
{
    while (1) {
        input();                           /* ����   */
        token = nextTkn();                 /* �ŏ��̃g�[�N�� */
        if (token.kind == EofTkn) exit(1); /* �I��   */
        statement();                       /* �����s */
        if (errF) puts("  --err--");
    }
    return 0;
}

void input(void)
{
    errF = 0; stkct = 0;                   /* �����ݒ�           */
    buf[0] = '\0';
    fgets(buf, 80, stdin);                 /* 80�����ȓ��̓���   */
    bufp = buf;                            /* �擪�����Ɉʒu�Â� */
    ch = nextCh();                         /* �ŏ��̕����擾     */
}

void statement(void)                       /* �� */
{
    int vNbr;

    switch (token.kind) {
    case VarName:                          /* �����     */
        vNbr = token.val;                  /* �����ۑ� */
        token = nextTkn();
        chkTkn(Assign); if (errF) break;   /* '=' �̂͂� */
        token = nextTkn();
        expression();                      /* �E�ӌv�Z   */
        var[vNbr] = pop();                 /* ������s   */
        break;
    case Print:                            /* print��(?) */
        token = nextTkn();
        expression();
        chkTkn(EofTkn); if (errF) break;
        printf("  %d\n", pop());
        return;
    default:
        errF = 1;
    }
    chkTkn(EofTkn);                        /* �s���`�F�b�N */
}

void expression(void)                      /* �� */
{
    Kind op;

    term();
    while (token.kind==Plus || token.kind==Minus) {
        op = token.kind;
        token = nextTkn(); term(); operate(op);
    }
}

void term(void)                            /* �� */
{
    Kind op;

    factor();
    while (token.kind==Multi || token.kind==Divi) {
        op = token.kind;
        token = nextTkn(); factor(); operate(op);
    }
}

void factor(void)                          /* ���q     */
{
    switch (token.kind) {
    case VarName:                          /* �ϐ�     */
        push(var[token.val]);
        break;
    case IntNum:                           /* �����萔 */
        push(token.val);
        break;
    case Lparen:                           /* ( �� )   */
        token = nextTkn();
        expression();
        chkTkn(Rparen);                    /* ) �̂͂� */
        break;
    default:
        errF = 1;
    }
    token = nextTkn();
}

Token nextTkn(void)                        /* ���g�[�N�� */
{
    int num;
    Token tk = {Others, 0};

    while (isspace(ch))                    /* �󔒓ǂݎ̂� */
        ch = nextCh();
    if (isdigit(ch)) {                     /* ���� */
        for (num=0; isdigit(ch); ch = nextCh())
           num = num*10 + (ch-'0');
        tk.val = num;                      /* �l�i�[ */
        tk.kind = IntNum;
    }
    else if (islower(ch)) {                /* �ϐ� */
        tk.val = ch - 'a';                 /* �ϐ��ԍ�0-25 */
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

int nextCh(void)                           /* ����1���� */
{
    if (*bufp == '\0') return '\0'; else return *bufp++;
}

void operate(Kind op)                      /* ���Z���s */
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

void push(int n)                           /* �X�^�b�N�ύ� */
{
    if (errF) return;
    if (stkct+1 > STK_SIZ) { puts("stack overflow"); exit(1); }
    stack[++stkct] = n;
}

int pop(void)                              /* �X�^�b�N��o */
{
    if (errF) return 1;                    /* �G���[���͒P��1��Ԃ� */
    if (stkct < 1) { puts("stack underflow"); exit(1); }
    return stack[stkct--];
}

void chkTkn(Kind kd)                       /* �g�[�N����ʊm�F */
{
    if (token.kind != kd) errF = 1;        /* �s��v */
}
