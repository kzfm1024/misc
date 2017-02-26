#include <mpi.h>
#include <stdio.h>
// MPI サンプルプログラム。実行効率は考慮せず。
// n以下の素数を見つけ、その数を出力する

// パイプラインの方法を用いる: ノード0は、すべての奇数に着目し、
// （すなわち、2の倍数はすでに除去済みである)、3の倍数
// を除去して、残りをノード1に渡す。ノード1は、
// 5の倍数を除去して、ノード2に渡す。ノード2は、
// 残りの非素数を除いて、素数の個数を表示する
//

// コマンドライン引数は n とデバッグ用の待ちフラグである

#define PIPE_MSG 0 // チェックする数字を含むメッセージの型
//
#define END_MSG 1 // データの残りがないことを示すメッセージの型
//

int nnodes, // ノード番号
    n, // 2から2までのすべての素数を調べる
    me; // 本ノードの番号

init(int argc,char **argv) {
    int debugwait; // もし1なら、デバッガを接続するまで
    // 無限ループになる

    MPI_Init(&argc,&argv);
    n = atoi(argv[1]);
    debugwait = atoi(argv[2]);

    MPI_Comm_size(MPI_COMM_WORLD,&nnodes);
    MPI_Comm_rank(MPI_COMM_WORLD,&me);

    while (debugwait) ;
}

void node0() {
    int i,dummy,
        tocheck; // 次のノードに受け渡すための現在の数
    for (i = 1; i <= n/2; i++) {
        tocheck = 2 * i + 1;
        if (tocheck > n) break;
        if (tocheck % 3 > 0)
            MPI_Send(&tocheck,1,MPI_INT,1,PIPE_MSG,MPI_COMM_WORLD);
    }
    MPI_Send(&dummy,1,MPI_INT,1,END_MSG,MPI_COMM_WORLD);
}

void node1() {
    int tocheck, // ノード0から渡されたチェックする数
        dummy;
    MPI_Status status; // 下記参照

    while (1) {
        MPI_Recv(&tocheck,1,MPI_INT,0,MPI_ANY_TAG,
                 MPI_COMM_WORLD,&status);
        if (status.MPI_TAG == END_MSG) break;
        if (tocheck % 5 > 0)
            MPI_Send(&tocheck,1,MPI_INT,2,PIPE_MSG,MPI_COMM_WORLD);
    }
    // メッセージ本体でなく、メッセージ型として
    // データ終了の信号を送る
    MPI_Send(&dummy,1,MPI_INT,2,END_MSG,MPI_COMM_WORLD);
}

void node2() {
    int tocheck, // ノード1から渡されたチェックする数
        primecount,i,iscomposite;
    MPI_Status status;

    primecount = 3; // 素数2,3,5は、下記でカウントされないので、
    // あらかじめカウントしておく
    while (1) {
        MPI_Recv(&tocheck,1,MPI_INT,1,MPI_ANY_TAG,
                 MPI_COMM_WORLD,&status);
        if (status.MPI_TAG == END_MSG) break;
        iscomposite = 0;
        for (i = 7; i*i <= tocheck; i += 2)
            if (tocheck % i == 0) {
                iscomposite = 1;
                break;
            }
        if (!iscomposite) primecount++;
    }
    printf("number of primes = %d\n",primecount);
}

main(int argc,char **argv) {
    init(argc,argv);
    switch (me) {
    case 0: node0();
        break;
    case 1: node1();
        break;
    case 2: node2();
    };
    MPI_Finalize();
}

