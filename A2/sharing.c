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

int perform_buckets_computation(int, int, int);

int main (int argc, const char *argv[]) {
    int num_threads, num_samples, num_buckets;

    if (argc != 4) {
		printf("Invalid input! Usage: ./sharing <num_threads> <num_samples> <num_buckets> \n");
		return 1;
	} else {
        num_threads = atoi(argv[1]);
        num_samples = atoi(argv[2]);
        num_buckets = atoi(argv[3]);
	}
    
    set_clock();
    perform_buckets_computation(num_threads, num_samples, num_buckets);

    printf("Using %d threads: %d operations completed in %.4gs.\n", num_threads, num_samples, elapsed_time());
    return 0;
}

typedef struct {
int _c;
int _padding[15];
} PadInt;

int perform_buckets_computation(int num_threads, int num_samples, int num_buckets) {
    volatile int *histogram = (int*) calloc(num_buckets, sizeof(int));
    rand_gen generator = init_rand();
    PadInt hist[num_buckets][num_threads];
    omp_set_num_threads(num_threads);

    #pragma omp parallel for
    for(int i = 0; i < num_samples; i++){
        int val = next_rand(generator) * num_buckets;
        int tid = omp_get_thread_num();
        hist[val][tid]._c++;
    }
    free_rand(generator);

    #pragma omp parallel for shared(hist, histogram)
    for(int i = 0; i < num_buckets; i++) {
        for (int j = 0; j < num_threads; j++) {
        histogram[i] += hist[i][j]._c;
        }
    }

    return 0;
}
