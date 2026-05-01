#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"

void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads)
{
    // Put your code here.

    // Check correct dimensions
    if (A->n != B->m || A->m != C->m || B->n != C->n)
    {
        fprintf(stderr, "mat_multiply: Matrices sizes mismatch.\n");
        exit(1);
    }

    // Perform matrix multiplication with OpenMP, parallelizing the outer
    // loop distributes rows of C to different threads.
    //
    // - default(none)
    //   Every variable is thread-local unless explicitly marked as shared.
    //
    // - num_threads(threads)
    //   Set the number of threads to use.
    /*
   PERMUTATION 1: i → j → k (classic form)

   Outer loop: i (parallelized)
   Each thread computes full rows of C
   Good: simple, safe
   Bad: poor cache usage for B (column access)
   */

    /*
    #pragma omp parallel for default(none) shared(A,B,C,m,n,p) num_threads(threads)
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < p; j++) {
            double tmp = 0.0;
            for (unsigned int k = 0; k < n; k++) {
                tmp += A->ptr[i * n + k] * B->ptr[k * p + j];
            }
            C->ptr[i * p + j] = tmp;
        }
    }
    */

    /*
    PERMUTATION 2: i → k → j (BEST - ACTIVE)

    Outer loop: i (parallelized)
    Excellent cache locality:
    - A reused in registers
    - B accessed row-wise
    - C updated sequentially
    */

    /*
    #pragma omp parallel for default(none) shared(A,B,C,m,n,p) num_threads(threads)
    for (unsigned int i = 0; i < m; i++) {
        for (unsigned int j = 0; j < p; j++) {
            C->ptr[i * p + j] = 0.0;
        }

        for (unsigned int k = 0; k < n; k++) {
            double a_val = A->ptr[i * n + k];
            for (unsigned int j = 0; j < p; j++) {
                C->ptr[i * p + j] += a_val * B->ptr[k * p + j];
            }
        }
    }
    */

    /*
    PERMUTATION 3: j → i → k

    Outer loop: j (parallelized)
    Column-wise computation of C
    Poor locality for C and B
    */

    /*
    #pragma omp parallel for default(none) shared(A,B,C,m,n,p) num_threads(threads)
    for (unsigned int j = 0; j < p; j++) {
        for (unsigned int i = 0; i < m; i++) {
            double tmp = 0.0;
            for (unsigned int k = 0; k < n; k++) {
                tmp += A->ptr[i * n + k] * B->ptr[k * p + j];
            }
            C->ptr[i * p + j] = tmp;
        }
    }
    */

    /*
    PERMUTATION 4: j → k → i

    Outer loop: j (parallelized)
    Updates full column of C incrementally
    Requires initialization of C to 0
    */

    /*
    #pragma omp parallel for default(none) shared(A,B,C,m,n,p) num_threads(threads)
    for (unsigned int j = 0; j < p; j++) {
        for (unsigned int k = 0; k < n; k++) {
            double b_val = B->ptr[k * p + j];
            for (unsigned int i = 0; i < m; i++) {
                C->ptr[i * p + j] += A->ptr[i * n + k] * b_val;
            }
        }
    }
    */

    /*
    PERMUTATION 5: k → i → j

    Outer loop: k (parallelized)
    Multiple threads update same C[i][j] → race condition
    */

    /*
    #pragma omp parallel for default(none) shared(A,B,C,m,n,p) num_threads(threads)
    for (unsigned int k = 0; k < n; k++) {
        for (unsigned int i = 0; i < m; i++) {
            double a_val = A->ptr[i * n + k];
            for (unsigned int j = 0; j < p; j++) {
                #pragma omp atomic
                C->ptr[i * p + j] += a_val * B->ptr[k * p + j];
            }
        }
    }
    */

    /*
    PERMUTATION 6: k → j → i

    Outer loop: k (parallelized)
    Column updates with contention on C
    */

#pragma omp parallel for default(none) shared(A, B, C, m, n, p) num_threads(threads)
    for (unsigned int k = 0; k < n; k++)
    {
        for (unsigned int j = 0; j < p; j++)
        {
            double b_val = B->ptr[k * p + j];
            for (unsigned int i = 0; i < m; i++)
            {
#pragma omp atomic
                C->ptr[i * p + j] += A->ptr[i * n + k] * b_val;
            }
        }
    }
}