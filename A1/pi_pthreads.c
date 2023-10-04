/*
============================================================================
Filename    : pi_pthreads.c
Author      : Benedikt Heuser,
SCIPER		: 376973,
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "utility.h"

double calculate_pi (int num_threads, int samples);

typedef struct _thread_data_t {
  int tid;
  double num_of_iter;
  double count;
} thread_data_t;

// thread function
void *thr_func(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;

    int iterations = data->num_of_iter;
    rand_gen gen = init_rand_pthreads(data->tid);

    for (int i = 0; i < iterations; i++) {

        // generate new sample point
        double random_x = next_rand(gen);
        double random_y = next_rand(gen);

        // update counter
        if (random_x * random_x + random_y * random_y <= 1) {
            data->count++;
        }
    }

    free_rand(gen);
    pthread_exit(NULL);
}

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
    double pi = 0.0;

    pthread_t thr[num_threads];
    int rc;

    thread_data_t tdata[num_threads];

    // create threads
    for (int i = 0; i < num_threads; ++i) {
        tdata[i].tid = i;
        tdata[i].num_of_iter = samples / num_threads;
        tdata[i].count = 0;
        if ((rc = pthread_create(&thr[i], NULL, thr_func, &tdata[i]))) {
            fprintf(stderr, "error: couldn't create thread, rc: %d\n", rc);
        }
    }

    // wait for threads to finish
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(thr[i], NULL);
    }

    // count up all the results of the threads
    double num_inside = 0;
    for (int i = 0; i < num_threads; i++) {
        num_inside += tdata[i].count;
    }

    pi = 4 * num_inside / samples;

    return pi;
}
