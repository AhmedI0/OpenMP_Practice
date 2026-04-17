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



## Task 3: OpenMP Sections (MIMD Work Sharing)

In this task, I explored the use of OpenMP `sections`, which enable **MIMD-style parallelism** (Multiple Instructions, Multiple Data). Unlike `parallel for`, where all threads execute the same loop, `sections` allow different threads to execute completely different blocks of code.

---

### Objective

The goal of this task was to:
- understand how `#pragma omp sections` works
- debug a program that **hangs or behaves incorrectly**
- identify issues related to thread execution and synchronization
- fix the program to run correctly and terminate cleanly

---

### Code Structure

The program initializes two arrays `a[]` and `b[]`, and defines two independent computations:

- **Section 1**:
  ```c
  c[i] = a[i] * b[i];


(element-wise multiplication)

Section 2:

c[i] = a[i] + b[i];


(element wise addition)

These are executed inside:


#pragma omp sections
{
    #pragma omp section
    { ... }

    #pragma omp section
    { ... }
}



# Observed Problem


The original program:

- sometimes hung (never terminated)
- sometimes produced inconsistent output

The root cause was related to how sections assign work to threads.


Root Cause Analysis

In OpenMP sections:

each section is executed by one thread only
if there are more threads than sections, some threads:
do not execute any section
skip directly to the next part of the program

However, the original code assumed that all threads would execute a section:


while (1) {
    if (section == 1 || section == 2)
        break;
}




Problem:

threads that did not execute any section had an uninitialized section variable
these threads could get stuck in an infinite loop


# Fixes applied:

1. Proper thread identification
Replaced placeholder values:

tid = omp_get_thread_num();
nthreads = omp_get_num_threads();




2. Initialization of section

Added:

section = 0; 

This ensures:

Threads that execute no section have a defined state


3. Removed infinite loop

The probelmatic loop was removed entirely


while (1) { ... }

Reason:

It relied in incorrect  assumtions about thread participation



4. Safe Handling of Threads with No Work
Added:

if (section == 0) {
    printf("Thread %d executed no section.\n", tid);
}


5. Synchronization
Used barriers to maintain clean output:

#pragma omp barrier

and allowed implicit synchronization at the end of sections


Final Behavior

With OMP_NUM_THREADS=4, the program produces:

- 4 threads are created
- 2 threads execute the sections:
- one handles multiplication
- one handles addition
- remaining threads:
- execute no section
- continue safely
- all threads reach the end and terminate correctly


Key points:

- sections implement MIMD parallelism, not loop-based parallelism
- not all threads are guaranteed to receive work
- uninitialized variables in parallel regions can cause serious bugs
- synchronization and correct initialization are critical
- thread-safe code must also handle the case of idle threads


Conclusion:

Writing parallel code requires understanding both how work is assigned and how threads behave when
no work is assigned in sections


