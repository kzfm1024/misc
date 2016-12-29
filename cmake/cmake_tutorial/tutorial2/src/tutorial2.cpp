#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef USE_MYMATH
#include "mymath.h"
#endif

int main (int argc, char** argv)
{
    if (argc < 2)
    {
        fprintf(stdout,"Usage: %s number\n",argv[0]);
        return 1;
    }

    double inputValue = atof(argv[1]);

#ifdef USE_MYMATH
    double outputValue = mysqrt(inputValue);
#else
    double outputValue = sqrt(inputValue);
#endif

    printf("The square root of %g is %g\n", inputValue, outputValue);
    return 0;
}
