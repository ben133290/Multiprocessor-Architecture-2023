/*
============================================================================
Filename    : algorithm.c
Author      : Benedikt Heuser, Atharva Gangal
SCIPER		: 376973, 376328

============================================================================
*/
#include <math.h>

#define INPUT(I,J) input[(I)*length+(J)]
#define OUTPUT(I,J) output[(I)*length+(J)]

void simulate(double *input, double *output, int threads, int length, int iterations)
{
    double *temp;
    
    // Parallelize this!!
    for(int n=0; n < iterations; n++)
    {
	#pragma omp parallel for num_threads(threads)
        for(int i=1; i<length-1; i++)
        {
            #pragma omp parallel for num_threads(threads)
            for(int j=1; j<length-1; j++)
            {
                    if ( ((i == length/2-1) || (i== length/2))
                        && ((j == length/2-1) || (j == length/2)) )
                        continue;

                    OUTPUT(i,j) = (INPUT(i-1,j-1) + INPUT(i-1,j) + INPUT(i-1,j+1) +
                                   INPUT(i,j-1)   + INPUT(i,j)   + INPUT(i,j+1)   +
                                   INPUT(i+1,j-1) + INPUT(i+1,j) + INPUT(i+1,j+1) )/9;

            }
        }

        temp = input;
        input = output;
        output = temp;
    }
}
