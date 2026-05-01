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
    // #pragma omp parallel for default(none) shared(A, B, C) num_threads(threads)
    //     for (unsigned int i = 0; i < A->m; i++)
    //     {
    //         for (unsigned int j = 0; j < B->n; j++)
    //         {
    //             double tmp = 0.0;
    //             for (unsigned int k = 0; k < A->n; k++)
    //             {
    //                 tmp += A->ptr[i * A->n + k] * B->ptr[k * B->n + j];
    //             }
    //             C->ptr[i * C->n + j] = tmp;
    //         }
    //     }

    // #pragma omp parallel for default(none) shared(A, B, C) num_threads(threads)
    //     for (unsigned int i = 0; i < A->m; i++)
    //     {
    //         for (unsigned int k = 0; k < A->n; k++)
    //         {
    //             double a_val = A->ptr[i * A->n + k];
    //             for (unsigned int j = 0; j < B->n; j++)
    //             {
    //                 C->ptr[i * C->n + j] += a_val * B->ptr[k * B->n + j];
    //             }
    //         }
    //     }

    // #pragma omp parallel for default(none) shared(A, B, C) num_threads(threads)
    //     for (unsigned int j = 0; j < B->n; j++)
    //     {
    //         for (unsigned int i = 0; i < A->m; i++)
    //         {
    //             double tmp = 0.0;
    //             for (unsigned int k = 0; k < A->n; k++)
    //             {
    //                 tmp += A->ptr[i * A->n + k] * B->ptr[k * B->n + j];
    //             }
    //             C->ptr[i * C->n + j] = tmp;
    //         }
    //     }

    // #pragma omp parallel for default(none) shared(A, B, C) num_threads(threads)
    //     for (unsigned int j = 0; j < B->n; j++)
    //     {
    //         for (unsigned int k = 0; k < A->n; k++)
    //         {
    //             double b_val = B->ptr[k * B->n + j];
    //             for (unsigned int i = 0; i < A->m; i++)
    //             {
    //                 C->ptr[i * C->n + j] += A->ptr[i * A->n + k] * b_val;
    //             }
    //         }
    //     }

    // #pragma omp parallel for default(none) shared(A, B, C) num_threads(threads)
    //     for (unsigned int k = 0; k < A->n; k++)
    //     {
    //         for (unsigned int i = 0; i < A->m; i++)
    //         {
    //             double a_val = A->ptr[i * A->n + k];
    //             for (unsigned int j = 0; j < B->n; j++)
    //             {
    // #pragma omp atomic
    //                 C->ptr[i * C->n + j] += a_val * B->ptr[k * B->n + j];
    //             }
    //         }
    //     }

    // #pragma omp parallel for default(none) shared(A, B, C) num_threads(threads)
    //     for (unsigned int k = 0; k < A->n; k++)
    //     {
    //         for (unsigned int j = 0; j < B->n; j++)
    //         {
    //             double b_val = B->ptr[k * B->n + j];
    //             for (unsigned int i = 0; i < A->m; i++)
    //             {
    // #pragma omp atomic
    //                 C->ptr[i * C->n + j] += A->ptr[i * A->n + k] * b_val;
    //             }
    //         }
    //     }
}