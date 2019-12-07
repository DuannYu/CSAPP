#include "csapp.h"
#define MAXTHREADS 32

void *sum_mutex(void *vargv);   /* Thread routine */

/* Global shared variables */
long gsum = 0;                  /* Global sum */
long nelems_per_thread;         /* Number of elements sum */
long psum[MAXTHREADS];
sem_t mutex;                    /* Mutex to protect global sum */


int main(int argc, char **argv) {
    long nelems, long_nelems, nthreads, myid[MAXTHREADS];
    pthread_t tid[MAXTHREADS];

    /* Get input arguments */
    if (argc != 3) {
        printf("usage: %s <nthreads> <long_nelems>\n", argv[0]);
        exit(0);
    }

    nthreads = atol(argv[1]);
    long_nelems = atol(argv[2]);
    nelems = (1L << long_nelems);
    nelems_per_thread = nelems / nthreads;
    sem_init(&mutex, 0, 1);

    /* Create peer threads and wait for them finish */
    for (int i = 0; i < nthreads; i++) {
        myid[i] = i;
        pthread_create(&tid[i], NULL, sum_mutex, &myid[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < nthreads; i++) {
        gsum += psum[i];
    }
    /* Check final answer */
    if (gsum != (nelems * (nelems - 1))/2) {
        printf("Error: result=%ld\n", gsum);
    }
} 

void *sum_mutex(void *vargv) {
    long myid = *((long *)vargv);
    long start = myid * nelems_per_thread;
    long end = start + nelems_per_thread;
    long i, sum = 0;
    for (long i = start; i < end; i++) {
        sum += i;
    }
    psum[myid] = sum;

    return NULL;
}
