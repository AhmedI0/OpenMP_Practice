# OpenMP Practice

This repository contains my practice work from an HPC workshop focused on OpenMP programming.

## Overview

This project demonstrates:
- Basic OpenMP parallel regions
- Thread control using environment variables
- Race conditions and synchronization
- Use of `critical`, `atomic`, and `reduction`
- Loop-level parallelization
- Work-sharing vs incorrect parallelization

## Environment

Work was performed on an HPC cluster using GCC with OpenMP support.

## Files

- `e1_1.c` → Reduction and race condition handling
- `e1_2.c` → OpenMP directives, thread control, and loop parallelization

## Compilation

```bash
gcc -fopenmp -o e1_1_parallel e1_1.c
gcc -fopenmp -o e1_2_parallel e1_2.c

Execution:

export OMP_NUM_THREADS=4
./e1_1_parallel
./e1_2_parallel 6

Parallelizing the code requires both:

1. correct snchronization
2. correct work distribution

critical ensures saftey but not correctness of workload division
reduction is the preffered method for parallel summation
Parallel ouput order is not guaranteed. 



## Task 2: OpenMP Directives and Work Sharing

In this task, I explored different OpenMP directives and their effects on correctness and parallel behavior.

### Snippet #1 — Parallel Region
Used:
```c
#pragma omp parallel


Created multiple threads 
Printed thread IDs using omp_get_thread_num()
Controlled thread count using:

export OMP_NUM_THREADS=4

# Snippet #2:  Critical Section (Incorrect Work Sharing)


#pragma omp parallel
{
    local_sum = multiply(a,b,n);

    #pragma omp critical
    {
        sum += local_sum;
    }
}


Observations: critical ensures safe updates to sum However, each thread computes the full dot product This leads to 
duplicated work and incorrect result
 Example: Correct result: 20.625 With 4 threads: 82.5 


# Snippet #3 — Reduction 
(Correct Approach)

#pragma omp parallel for reduction(+:sum)

Loop iterations distributed across threads
Each loop computes a partial sum
OpenMP combines results automatically
correct and efficient


# snippet #4 - Scheduled parallel function

Implemented an alternative function

#pragma omp parallel for reduction(+:local_sum) schedule(static)

Allows explicit control over work distribution
Schedule(static) assigns fixed chunks of iterations to threads
Produces correct result

Synchronization (critical) alone does not guarantee correct parallelization
Proper work decomposition is essential
reduction is the preferred method for parallel summation
Parallel output order is not deterministic
Scheduling strategies can influence performance and load balancing
