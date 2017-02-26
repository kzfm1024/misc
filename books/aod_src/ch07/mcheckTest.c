#include <stdlib.h>
#include <mcheck.h>

int main(void)
{
    mcheck(NULL); 
    int *p = (int *)malloc(sizeof(int));
    p[1] = 0;
    free(p);
    return 0;
}
