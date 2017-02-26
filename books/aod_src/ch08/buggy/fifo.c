// fifo.c, SWIGの例; 文字のFIFO キューを管理する
#include <stdlib.h>
#include <string.h>
char *fifo; // キュー

int nfifo = 0, // 現在のキューの長さ
    maxfifo; // キューの最大長

int fifoinit(int spfifo) { // 最大spfifo要素の領域を確保する
    fifo = malloc(spfifo);
    if (fifo == 0) return 0; // 失敗
    else {
        maxfifo = spfifo;
        return 1; // 成功
    }
}

int fifoput(char c) { // cをキューに追加する
    if (nfifo < maxfifo) {
        fifo[nfifo] = c;
        return 1; // 成功
    }
    else return 0; // 失敗
}

char fifoget() { // キューの先頭を取り出して返す
    char c;
    if (nfifo > 0) {
        c = fifo[0];
        memmove(fifo,fifo+1,--nfifo);
        return c;
    }
    else return 0; // NULL文字はキューにないことを仮定
}
