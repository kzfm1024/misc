#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <mcheck.h>

void sigsegv_handler(int signum);

int main(void)
{
    int *p;
    signal(SIGSEGV, sigsegv_handler);
    mtrace();
    p = (int *)malloc(sizeof(int));
    raise(SIGSEGV);
    return 0;
}
void sigsegv_handler(int signum)
{
    printf("Caught sigsegv: signal %d. Shutting down gracefully.\n", signum);
    muntrace();
    abort();
}
