// dijkstra.c

// OpenMP例題プログラム：ダイクストラ法により無向グラフ上の
// 最短経路解法。頂点0と他の頂点の間の最短経路を求める。
//

// 使い方: dijkstra nv print

// ここで nv はグラフの大きさ。print が 1なら、
// 最短距離が表示される。0なら何もしない。

#include <omp.h> // 必要
#include <values.h>

// stdlib.h と stdio.h をインクルードすると、Omniコンパイラと
// 衝突するので、直接宣言する。
extern void *malloc();
extern int printf(char *,...);

// すべてのスレッドで共有されるグローバル変数
int nv, // 頂点数
    *notdone, // チェックされていない頂点
    nth, // スレッド数
    chunk, // 各スレッドで処理する頂点数
    md, // 全スレッドでの最小値
    mv; // 最小値をもたらす頂点番号

int *ohd, // 頂点間の連接距離;
          // "ohd[i][j]" は、ohd[i*nv+j]となる
    *mind; // 求められた最小距離

void init(int ac, char **av) {
    int i,j,tmp;
    nv = atoi(av[1]);
    ohd = malloc(nv*nv*sizeof(int));
    mind = malloc(nv*sizeof(int));
    notdone = malloc(nv*sizeof(int));
    // ランダムグラフの生成
    for (i = 0; i < nv; i++)
        for (j = i; j < nv; j++) {
            if (j == i) ohd[i*nv+i] = 0;
            else {
                ohd[nv*i+j] = rand() % 20;
                ohd[nv*j+i] = ohd[nv*i+j];
            }
        }
    for (i = 1; i < nv; i++) {
        notdone[i] = 1;
        mind[i] = ohd[i];
    }
}

// sからeまでのnotdoneの中で、もっとも0に近いものを見つけ、
// 最短距離を*dに、最短頂点を *vに返す。
void findmymin(int s, int e, int *d, int *v) {
    int i;
    *d = MAXINT;
    for (i = s; i <= e; i++)
        if (notdone[i] && mind[i] < *d) {
            *d = mind[i];
            *v = i;
        }
}

// sからeまでのiについて、mからiまで短い経路があるかどうか
// 調べる
void updatemind(int s, int e) {
    int i;
    for (i = s; i <= e; i++)
        if (notdone[i])
            if (mind[mv] + ohd[mv*nv+i] < mind[i])
                mind[i] = mind[mv] + ohd[mv*nv+i];
}

void dowork() {

    #pragma omp parallel
    { int startv,endv, // このスレッドの開始、終了頂点
          step, // 0からnv未満までのstepを調べる
          mymv, // この値まで到達する頂点
          me = omp_get_thread_num(),
          mymd; // このスレッドで見つかった最小値
        #pragma omp single
        { nth = omp_get_num_threads(); chunk = nv/nth;
            printf("there are %d threads\n",nth); }
        startv = me * chunk;
        endv = startv + chunk - 1;
        // nvまで繰り返すアルゴリズム
        for (step = 0; step < nv; step++) {
            // notdoneの中でもっとも0に近い頂点を求める。各スレッドは
            // グループの中で最近点を求め、ここで全体から最近点を求める
            #pragma omp single
            { md = MAXINT;
                mv = 0;
            }
            findmymin(startv,endv,&mymd,&mymv);
            // このスレッドのものが最小なら全体の最小値を更新する
            #pragma omp critical
            { if (mymd < md)
                { md = mymd; }
            }
            #pragma omp barrier
            // 新しい頂点に処理済みの印をつける
            #pragma omp single
            { notdone[mv] = 0; }
            // ohdの区間を更新する
            updatemind(startv,endv);
        }
    }
}

int main(int argc, char **argv) {
    int i,j,print;
    init(argc,argv);
    // 並列処理を開始する
    dowork();
    // シングルスレッドに戻る
    print = atoi(argv[2]);
    if (print) {
        printf("graph weights:\n");
        for (i = 0; i < nv; i++) {
            for (j = 0; j < nv; j++)
                printf("%u ",ohd[nv*i+j]);
            printf("\n");
        }
        printf("minimum distances:\n");
        for (i = 1; i < nv; i++)
            printf("%u\n",mind[i]);
    }
}
