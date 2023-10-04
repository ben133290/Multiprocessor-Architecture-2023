/*
============================================================================
Filename    : integral.c
Author      : Benedikt Heuser
SCIPER		: 376973,
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
    double integral = 0;

    omp_set_num_threads(num_threads);

    /* Your code goes here */
    rand_gen gen = init_rand();
    double x = 0;
    double fx = 0;
    double range = b - a;
    double sum_of_rects = 0;

    #pragma omp parallel for
    for (int i = 0; i < samples; i++) {

        x = next_rand(gen) * range + a;
        fx = f(x);

        #pragma omp atomic
        sum_of_rects = sum_of_rects + fx * range;

    }

    integral = sum_of_rects / samples;

    return integral;
}
