#include <iostream>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
using namespace tbb;

static const int NUM = 10;
float s_input[NUM];
float s_output[NUM];

struct Average
{
    const float* input;
    float* output;
    void operator()( const blocked_range<int>& range ) const
        {
            for (int i = range.begin(); i!=range.end(); ++i)
            {
                output[i] = (input[i-1]+input[i]+input[i+1])*(1/3.f);
            }
        }
};

void ParallelAverage( float* output, const float* input, size_t n )
{
    Average avg;
    avg.input = input;
    avg.output = output;
    parallel_for( blocked_range<int>( 1, n ), avg );
}

main()
{
    ParallelAverage(s_input, s_output, NUM);
}
