// srvr.c

// リモートで wを実行し、ログイン権限がなくても
// サーバの負荷を調べることができるサーバプログラム
// 使い方: svr

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define WPORT 2000
#define BUFSIZE 1000 // これで十分と仮定

int clntdesc, // クライアントのソケット記述子
svrdesc; // サーバの一般ソケット記述子

char outbuf[BUFSIZE]; // クライアントへのメッセージ

void respond()
{
    int fd,nb;
    memset(outbuf,0,sizeof(outbuf)); // バッファを消去
    system("w > tmp.client"); // 'w'を実行し結果を保存する
    fd = open("tmp.client",O_RDONLY);
    nb = read(fd,outbuf,BUFSIZE); // ファイル全体を読み込む
    write(clntdesc,outbuf,nb); // クライアントに返す
    close(fd); unlink("tmp.client"); // ファイルを削除する
    close(clntdesc);
}

int main()
{
    struct sockaddr_in bindinfo;
    int one = 1;
    // 接続を受けつけるためのソケットを作成する
    svrdesc = socket(AF_INET,SOCK_STREAM,0);
    setsockopt(svrdesc, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    bindinfo.sin_family = AF_INET;
    bindinfo.sin_port = WPORT;
    bindinfo.sin_addr.s_addr = INADDR_ANY;
    bind(svrdesc,(struct sockaddr *) &bindinfo,sizeof(bindinfo));
    
    // OK。ループでクライアントからの呼び出しを受けつける
    listen(svrdesc,5);
    
    while (1) {
        // 呼び出しを待つ
        clntdesc = accept(svrdesc,0,0);
        // コマンドを実行する
        respond();
    }
}
