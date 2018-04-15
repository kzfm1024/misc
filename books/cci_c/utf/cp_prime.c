/*---------------------------*/
/*  素数を求める cp_prime.c  */
/*---------------------------*/
int is_prime(int n)  /* nが素数なら真を返す */
{
    int i;

    if (n < 2)    return 0;     /* 2未満は素数でない       */
    if (n == 2)   return 1;     /* 2は素数である           */
    if (n%2 == 0) return 0;     /* 2以外の偶数は素数でない */

    for (i=3; i*i<=n; i=i+2) {
        if (n%i == 0) return 0; /* 割り切れたら素数でない */
    }
    return 1;                   /* 素数である */
}

int main(void)
{
    int n;

    for (n=1; n<=1000; n++) {   /* 1000以下の素数は */
        if (is_prime(n)) {
            printf("%d ", n);
        }
    }
    printf("\n");

    return 0;
}

