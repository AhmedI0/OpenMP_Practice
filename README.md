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
