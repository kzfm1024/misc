/*-----------------------------------------*/
/*    �t�|�[�����h�L�@�̕]�� polish_p.c    */
/*-----------------------------------------*/
#include <stdio.h>
#include <stdlib.h>   /* for exit()   */
#include <ctype.h>    /* for is...()  */

void polish(char *s);
int execute(void);
int getvalue(int ch);
int order(int ch);
void push(int n);
int pop(void);

#define STK_SIZ 20
int stack[STK_SIZ+1];                                    /* �X�^�b�N */
int stkct;                                       /* �X�^�b�N�|�C���^ */
char plsh_out[80];                             /* �t�|�[�����h�o�͐� */

int main(void)
{
    char siki[80];
    int ans;

    printf("����: "); fgets(siki, 80, stdin);
    polish(siki);
    if (plsh_out[0] == '\0') exit(1);  /* �L���Ȏ����Ȃ� */
    ans = execute();
    printf("�ϊ�: %s\n", plsh_out);
    printf("����: %d\n", ans);

    return 0;
}

void polish(char *s)
{
    int wk;
    char *out = plsh_out;

    stkct = 0;
    while  (1) {
        while (isspace(*s)) { ++s; }           /* �󔒃X�L�b�v       */
        if (*s == '\0') {                      /* �����ɂȂ���       */
            while (stkct > 0) {                /* �X�^�b�N�c����o�� */
                if ((*out++ = pop()) == '(') {
                    puts("'('���s��\n"); exit(1);
                }
            }
            break;
        }
        if (islower(*s) || isdigit(*s)) {       /* �ϐ��������Ȃ�o��*/
            *out++ = *s++; continue;
        }
        switch (*s) {
        case '(':                              /* ( �Ȃ�ς�         */
            push(*s);
            break;
        case ')':                              /* ) �Ȃ�             */
            while ((wk = pop()) != '(') {      /* ( �ɉ�܂ŏo��   */
                *out++ = wk;
                if (stkct == 0) { puts("'('���s��\n"); exit(1); }
            }
            break;
        default:                               /* ���Z�q             */
            if (order(*s) == -1) {             /* ���p�s�K����       */
                printf("�s���ȕ���(%c)\n", *s); exit(1);
            }            /* �����ȏ㋭���v�f���X�^�b�N�g�b�v�ɂ���� */
            while (stkct>0 && (order(stack[stkct]) >= order(*s))) {
                *out++ = pop();                /* �X�^�b�N���e���o�� */
            }
            push(*s);                          /* ���v�f��ς�       */
        }
        s++;
    }
    *out = '\0';
}

int execute(void)                                        /* ���̌v�Z */
{
    int d1, d2;
    char *s;

    stkct = 0;
    for (s=plsh_out; *s; s++) {
        if (islower(*s))                       /* �ϐ��Ȃ�           */
            push(getvalue(*s));                /* �l���X�^�b�N�ɐς� */
        else if (isdigit(*s))                  /* �����Ȃ�           */
            push(*s - '0');                    /* �l���X�^�b�N�ɐς� */
        else {                                 /* ���Z�q�Ȃ�         */
            d2 = pop(); d1 = pop();            /* 2�̒l�����o��  */
            switch (*s) {
                case '+': push(d1+d2); break;  /* �v�Z���ʂ�ς�     */
                case '-': push(d1-d2); break;
                case '*': push(d1*d2); break;
                case '/': if (d2 == 0) {  puts("�[�����Z"); exit(1); }
                          push(d1/d2); break;
            }
        }
    }
    if (stkct != 1) { printf("error\n"); exit(1); }
    return pop();                              /* ���ʂ�Ԃ�        */
}

int getvalue(int ch)                                 /* 1�`26��Ԃ� */
{
    if (islower(ch)) return ch-'a' + 1;
    return 0;
}

int order(int ch)                                        /* �D�揇�� */
{
    switch (ch) {
    case '*': case '/': return 3;
    case '+': case '-': return 2;
    case '(':           return 1;
    }
    return -1;
}

void push(int n)                                     /* �X�^�b�N�ύ� */
{
    if (stkct+1 > STK_SIZ) { puts("stack overflow"); exit(1); }
    stack[++stkct] = n;
}

int pop(void)                                        /* �X�^�b�N��o */
{
    if (stkct < 1) { puts("stack underflow"); exit(1); }
    return stack[stkct--];
}
