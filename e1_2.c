#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double multiply(double a[], double b[], int n);
double multiply_sched(double a[], double b[], int n);

// Execute as: ./e1_2 6
// (here 6 is the command line argument for the vector size)
int main(int argc, const char *argv[])
{
    double sum;
    int i;

    if (argc != 2) {
        printf("Please pass the vector size as a command line argument\n");
        return 1;
    }

    int n = atoi(argv[1]);
    double a[n], b[n];

    // Snippet#1:
    #pragma omp parallel
    {
        printf("Hello, World! from Thread %d\n", omp_get_thread_num());
    }

    // Populating the vectors
    for (i = 0; i < n; i++) {
        a[i] = i * 0.25;
        b[i] = i * 1.5;
    }

    sum = 0.0;
    // Snippet#2:
    #pragma omp parallel
    {
        double local_sum = 0.0;
        local_sum = multiply(a, b, n);

        #pragma omp critical
        {
            sum += local_sum;
        }
    }
    printf("Snippet#2: sum = %f\n", sum);

    sum = 0.0;
    // Snippet#3:
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < n; i++) {
        sum += a[i] * b[i];
    }
    printf("Snippet#3: sum = %f\n", sum);

    sum = 0.0;
    // Snippet#4:
    sum = multiply_sched(a, b, n);
    printf("Snippet#4 (schedule static): sum = %f\n", sum);

    return 0;
}

double multiply(double a[], double b[], int n)
{
    int i;
    double local_sum = 0.0;

    for (i = 0; i < n; i++) {
        local_sum += a[i] * b[i];
    }

    return local_sum;
}

double multiply_sched(double a[], double b[], int n)
{
    int i;
    double local_sum = 0.0;

    #pragma omp parallel for reduction(+:local_sum) schedule(static)
    for (i = 0; i < n; i++) {
        local_sum += a[i] * b[i];
    }

    return local_sum;
}
