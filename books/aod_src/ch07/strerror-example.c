#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void)
{
    close(5);
    printf("%s\n", strerror(errno));
    return 0;
}
