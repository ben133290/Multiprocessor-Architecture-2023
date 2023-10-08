/*
============================================================================
Filename    : integral.c
Author      : Benedikt Heuser, Atharva Gangal
SCIPER		: 376973, 376328
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "function.c"

double integrate (int num_threads, int samples, int a, int b, double (*f)(double));

int main (int argc, const char *argv[]) {

    int num_threads, num_samples, a, b;
    double integral;

    if (argc != 5) {
		printf("Invalid input! Usage: ./integral <num_threads> <num_samples> <a> <b>\n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
        a = atoi(argv[3]);
        b = atoi(argv[4]);
	}

    set_clock();

    /* You can use your self-defined funtions by replacing identity_f. */
    integral = integrate (num_threads, num_samples, a, b, identity_f);

    printf("- Using %d threads: integral on [%d,%d] = %.15g computed in %.4gs.\n", num_threads, a, b, integral, elapsed_time());

    return 0;
}


double integrate (int num_threads, int samples, int a, int b, double (*f)(double)) {
    double integral;

    double range = b - a;
    double sum_of_rects = 0;

    #pragma omp parallel reduction(+:sum_of_rects)
    {
        rand_gen gen = init_rand();

        #pragma omp for 
        for(int i = 1; i <= samples; ++i){
            // generate new sample point
            double x = next_rand(gen) * range + a;
            double fx = f(x);

            sum_of_rects = sum_of_rects + fx * range;
            
        }

        free_rand(gen);
    } 
    
    integral = sum_of_rects / samples;

    return integral;
}
