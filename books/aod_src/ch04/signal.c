#include <signal.h>
#include <stdio.h>

void my_sigint_handler( int signum )
{
    printf("I received signal %d (that's 'SIGINT' to you).\n", signum);
    // %d シグナルを受け取りました（SIGINTです）
    puts("Tee Hee! That tickles!\n"); // キャハハ、くすぐったい
}

int main(void)
{
    char choicestr[20];
    int choice;
    
    while ( 1 )
    {
        puts("1. Ignore control-C"); // CTRL-Cを無視
        puts("2. Custom handle control-C"); // 独自のCTRL-Cハンドラ
        puts("3. Use the default handler control-C"); // デフォルトのCTRL-Cハンドラ
        puts("4. Raise a SIGSEGV on myself."); // SIGSEGVを発生
        printf("Enter your choice: "); // 選んでください：

        fgets(choicestr, 20, stdin);
        sscanf(choicestr, "%d", &choice);

        if ( choice == 1 )
            signal(SIGINT, SIG_IGN); // CTRL-Cを無視する
        else if ( choice == 2 )
            signal(SIGINT, my_sigint_handler);
        else if ( choice == 3 )
            signal(SIGINT, SIG_DFL);
        else if ( choice == 4 )
            raise(SIGSEGV);
        else
            puts("Whatever you say, guv'nor.\n\n"); // 旦那さまの仰せのままに
    }
    
    return 0;
}
