#include <stdio.h>
#include <stdlib.h>

int main( void )
{
    int *a = (int *) malloc( 3*sizeof(int) ); // mallocの返り値がチェックされない
    int *b = (int *) malloc( 3*sizeof(int) ); // mallocの返り値がチェックされない

    for (int i = -1; i <= 3; ++i)
        a[i] = i; //i = -1 と 3の書き込みは誤り

    free(a);
    printf("%d\n", a[1]); // 解放されたメモリを読む
    free(a); // ポインタaを2回解放する

    return 0; // *bを解放せずにプログラムを終了する
}
