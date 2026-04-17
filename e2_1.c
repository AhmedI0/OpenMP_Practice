#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Execute as: ./e2_1 6
// (command line parameter assigns size of two square matrices to be multiplied)

int main(int argc, char *argv[])
{
    long **m1, **m2, **prod;
    int i, j, k;
    int r1, c1, r2, c2;
    int thread_id, number_of_threads;

    if (argc != 2) {
        printf("Please provide one command line argument for matrix size.\n");
        return 1;
    }

    r1 = c1 = r2 = c2 = atoi(argv[1]);   // square matrices only

    // Allocate memory for matrix 1
    m1 = (long **) malloc(r1 * sizeof(long *));
    for (i = 0; i < r1; i++) {
        m1[i] = (long *) malloc(c1 * sizeof(long));
    }

    // Allocate memory for matrix 2
    m2 = (long **) malloc(r2 * sizeof(long *));
    for (i = 0; i < r2; i++) {
        m2[i] = (long *) malloc(c2 * sizeof(long));
    }

    // Allocate memory for product matrix
    prod = (long **) malloc(r1 * sizeof(long *));
    for (i = 0; i < r1; i++) {
        prod[i] = (long *) malloc(c2 * sizeof(long));
    }

    // Populate matrices
    printf("Populating the matrices ...\n");

    printf("Matrix 1:\n");
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c1; j++) {
            m1[i][j] = j + 1;
            printf("%ld\t", m1[i][j]);
        }
        printf("\n");
    }

    printf("Matrix 2:\n");
    for (i = 0; i < r2; i++) {
        for (j = 0; j < c2; j++) {
            m2[i][j] = j + 2;
            printf("%ld\t", m2[i][j]);
        }
        printf("\n");
    }

    // Parallel matrix multiplication
    #pragma omp parallel private(thread_id, i, j, k) shared(m1, m2, prod, r1, c1, c2, number_of_threads)
    {
        thread_id = omp_get_thread_num();

        if (thread_id == 0) {
            number_of_threads = omp_get_num_threads();
            printf("Number of threads = %d\n", number_of_threads);
        }

        #pragma omp for schedule(static)
        for (i = 0; i < r1; i++) {
            for (j = 0; j < c2; j++) {
                prod[i][j] = 0;
                for (k = 0; k < c1; k++) {
                    prod[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
    }

    // Print result
    printf("Result matrix:\n");
    for (i = 0; i < r1; i++) {
        for (j = 0; j < c2; j++) {
            printf("%ld\t", prod[i][j]);
        }
        printf("\n");
    }

    // Free memory
    for (i = 0; i < r1; i++) free(m1[i]);
    free(m1);

    for (i = 0; i < r2; i++) free(m2[i]);
    free(m2);

    for (i = 0; i < r1; i++) free(prod[i]);
    free(prod);

    return 0;
}
