/*
============================================================================
Filename    : pi.c
Author      : Benedikt Heuser,
SCIPER		: 376973,
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

double calculate_pi (int num_threads, int samples);

int main (int argc, const char *argv[]) {

    int num_threads, num_samples;
    double pi;

    if (argc != 3) {
		printf("Invalid input! Usage: ./pi <num_threads> <num_samples> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
	}

    set_clock();
    pi = calculate_pi (num_threads, num_samples);

    printf("- Using %d threads: pi = %.15g computed in %.4gs.\n", num_threads, pi, elapsed_time());

    return 0;
}


double calculate_pi (int num_threads, int samples) {
    double pi;

    /* Your code goes here */

    double num_of_samples_inside;

    rand_gen rand_gen = init_rand();

    omp_set_num_threads(num_threads);
    
    #pragma omp parallel for
    for (int i = 0; i < samples; i++) {

        // generate new sample point
        double random_x = next_rand(rand_gen);
        double random_y = next_rand(rand_gen);

        // update counter
        if (random_x * random_x + random_y * random_y <= 1) {
            #pragma omp atomic
            num_of_samples_inside++;
        }
    }

    // calculate pi
    pi = 4.0 * num_of_samples_inside / samples;

    free_rand(rand_gen);

    return pi;
}

