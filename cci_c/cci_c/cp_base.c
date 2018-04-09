/*----------------------------*/
/*  Šî–{ƒvƒƒOƒ‰ƒ€ cp_base.c  */
/*----------------------------*/
int add(int a, int b)
{
    int sum;
    sum = a + b;
    return sum;
}

int main(void)
{
    int ans;
    ans = add(10, 20);
    printf("%d\n", ans);
    return 0;
}

