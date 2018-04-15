/*-----------------------------*/
/*  足し算プログラム cp_add.c  */
/*-----------------------------*/
int main(void)
{
    int n, sum;

    sum = 0;
    printf("数値を入力してください。0入力で終了します。\n");
    while ((n=input()) != 0) {
        sum = sum + n;
        printf("    >%d\n", sum);
    }
    return 0;
}
