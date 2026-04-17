#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 50

void print_results(float array[N], int tid, int section);

int main(int argc, char *argv[]) {
    int i, nthreads, tid, section;
    float a[N], b[N], c[N];

    /* Some initializations */
    for (i = 0; i < N; i++)
        a[i] = b[i] = i * 1.0;

    #pragma omp parallel private(c,i,tid,section) shared(nthreads,a,b)
    {
        tid = omp_get_thread_num();
        section = 0;

        if (tid == 0) {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

        /*** Use barriers for clean output ***/
        #pragma omp barrier
        printf("Thread %d starting...\n", tid);
        #pragma omp barrier

        #pragma omp sections
        {
            #pragma omp section
            {
                section = 1;
                for (i = 0; i < N; i++)
                    c[i] = a[i] * b[i];
                print_results(c, tid, section);
            }

            #pragma omp section
            {
                section = 2;
                for (i = 0; i < N; i++)
                    c[i] = a[i] + b[i];
                print_results(c, tid, section);
            }
        }  /* end of sections */

        if (section == 0) {
            printf("Thread %d executed no section.\n", tid);
        }

        #pragma omp barrier
        printf("Thread %d done.\n", tid);
    }

    return 0;
}

void print_results(float array[N], int tid, int section) {
    int i;
    printf("Thread %d did section %d. First five results: ", tid, section);
    for (i = 0; i < 5; i++)
        printf("%.1f ", array[i]);
    printf("\n");
}
