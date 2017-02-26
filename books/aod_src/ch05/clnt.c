// clnt.c

// 使い方: clnt server_machine

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define WPORT 2000 // サーバのポート番号
#define BUFSIZE 1000

int main(int argc, char **argv)
{
    int sd,msgsize;
    struct sockaddr_in addr;
    struct hostent *hostptr;
    char buf[BUFSIZE];
    
    // ソケットの作成
    sd = socket(AF_INET,SOCK_STREAM,0);
    addr.sin_family = AF_INET;
    addr.sin_port = WPORT;
    hostptr = gethostbyname(argv[1]);
    memcpy(&addr.sin_addr.s_addr,hostptr->h_addr_list[0],hostptr->h_length);
    
    // OK。接続する
    connect(sd,(struct sockaddr *) &addr,sizeof(addr));
    
    // 返事を読んで、表示する
    msgsize = read(sd,buf,BUFSIZE);
    if (msgsize > 0)
        write(1,buf,msgsize);
    printf("\n");
    return 0;
}
