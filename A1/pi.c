/*
============================================================================
Filename    : pi.c
Author      : Benedikt Heuser, Atharva Gangal
SCIPER		: 376973, 376328
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


double calculate_pi (int num_threads, int samples){

    double pi;

    omp_set_num_threads(num_threads);

    int tid;
    long num_of_samples_inside = 0;

    #pragma omp parallel private(tid) reduction(+:num_of_samples_inside)
    {
    rand_gen gen = init_rand();

    #pragma omp for 
    for(int i = 1; i <= samples; ++i)
    {
        // generate new sample point
        double random_x = next_rand(gen);
        double random_y = next_rand(gen);

        // update counter
        if (random_x * random_x + random_y * random_y <= 1) {
            num_of_samples_inside++;
        }
    }

    free_rand(gen);
    } 
    
    pi = 4.0 * num_of_samples_inside / samples;
    return pi;

}
