/*-----------------------------*/
/*  �����Z�v���O���� cp_add.c  */
/*-----------------------------*/
int main(void)
{
    int n, sum;

    sum = 0;
    printf("���l����͂��Ă��������B0���͂ŏI�����܂��B\n");
    while ((n=input()) != 0) {
        sum = sum + n;
        printf("    >%d\n", sum);
    }
    return 0;
}
