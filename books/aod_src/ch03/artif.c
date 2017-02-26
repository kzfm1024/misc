#include <stdlib.h>
int *x;
main()
{
    x = (int *)malloc(25 * sizeof(int));
    x[3] = 12;
}
