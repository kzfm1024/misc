// 2からnまでの素数を見つける。エラトステネスのふるいを用いる。
// すべての2の倍数を削除し、すべての3の倍数を削除し、すべての5の倍数を削除し、
// と続ける。効率は悪いが、各スレッドは次の基数に移る前に、
// すべての倍数を削除する必要がある。

// 使い方: sieve n nthreads
// ただし nthreads はスレッドの数

#include <stdio.h>
#include <math.h>
#include <pthread.h>

#define MAX_N 100000000
#define MAX_THREADS 100

// 共有変数
int nthreads, // スレッド数（main()は除く）
    n, // 整数を探す上限
    prime[MAX_N+1], // 最終的にiが素数なら prime[i] = 1 、そうでなければ 0
    nextbase; // 次の倍数のふるい

int work[MAX_THREADS]; // 倍数のふるいの観点で、
// スレッドの働きを計測する。

// 共有変数 nextbase のロック用
pthread_mutex_t nextbaselock = PTHREAD_MUTEX_INITIALIZER;

// スレッドのID構造体
pthread_t id[MAX_THREADS];

// kの倍数を削除する。k*kから開始する
void crossout(int k)
{
    int i;
    for (i = k; i*k <= n; i++) {
        prime[i*k] = 0;
    }
}

// スレッドルーチン
void *worker(int tn) // tn はスレッド番号 (0,1,...)
{ int lim,base;

    // sqrt(n)より大きい倍数を調べる必要はない
    lim = sqrt(n);

    do {
        // スレッド間で重複しないように次の倍数を選ぶ
        pthread_mutex_lock(&nextbaselock);
        base = nextbase += 2;
        pthread_mutex_unlock(&nextbaselock);
        if (base <= lim) {
            work[tn]++; // このスレッドの働きを記録する
            //
            // 基数が素数ではないときに、倍数を消す必要はない
            if (prime[base])
                crossout(base);
        }
        else return;
    } while (1);
}

main(int argc, char **argv)
{
    int nprimes, // 見つかった素数の数
        totwork, // 調べた基数の数
        i;
    void *p;
    n = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    for (i = 2; i <= n; i++)
        prime[i] = 1;
    crossout(2);
    nextbase = 1;
    // スレッドを開始する
    for (i = 0; i < nthreads; i++) {
        pthread_create(&id[i],NULL,(void *) worker,(void *) i);
    }

    // すべてが終了するのを待つ
    totwork = 0;
    for (i = 0; i < nthreads; i++) {
        pthread_join(id[i],&p);
        printf("%d values of base done\n",work[i]);
        totwork += work[i];
    }
    printf("%d total values of base done\n",totwork);

    // 結果を報告する
    nprimes = 0;
    for (i = 2; i <= n; i++)
        if (prime[i]) { nprimes++; }
    printf("the number of primes found was %d\n",nprimes);

}
