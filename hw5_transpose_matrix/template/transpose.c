#include "util.h"
#include <omp.h>

void mat_sqtrans_seq(Mat *mat)
{
    unsigned int n = mat->n;

    for (unsigned int i = 0; i < n; i++)
    {
        for (unsigned int j = i + 1; j < n; j++)
        {
            double temp = mat->ptr[i * n + j];
            mat->ptr[i * n + j] = mat->ptr[j * n + i];
            mat->ptr[j * n + i] = temp;
        }
    }
}

/*
 * Decode a flat index k (0 <= k < n*(n-1)/2) into the corresponding
 * upper-triangular coordinates (i, j) with j > i.
 *
 * Uses the closed-form inverse of the triangular number formula to avoid
 * the slow while-loop in the original, which caused heavy contention when
 * many threads decoded their index simultaneously.
 */
static inline void decode_index(unsigned int k, unsigned int n,
                                 unsigned int *out_i, unsigned int *out_j)
{
    /*
     * Row i contains (n-1), (n-2), ... elements.
     * Cumulative elements before row i = i*n - i*(i+1)/2.
     * Solving for i given k via the quadratic formula gives:
     *   i = floor((2n - 1 - sqrt((2n-1)^2 - 8k)) / 2)
     */
    double fn = (double)n;
    double fk = (double)k;
    unsigned int i = (unsigned int)((2.0 * fn - 1.0 -
                      __builtin_sqrt((2.0 * fn - 1.0) * (2.0 * fn - 1.0) -
                                     8.0 * fk)) / 2.0);

    /* Correct for floating-point rounding (off by one at boundaries) */
    unsigned int used = i * n - i * (i + 1) / 2;
    if (used + (n - i - 1) <= k) i++;

    unsigned int j = i + 1 + (k - (i * n - i * (i + 1) / 2));
    *out_i = i;
    *out_j = j;
}

void mat_sqtrans_par(Mat *mat, unsigned int grain, unsigned int threads)
{
    unsigned int n     = mat->n;
    unsigned int total = n * (n - 1) / 2;

    /*
     * Shared work counter. Each thread atomically captures the next
     * chunk of `grain` work items, then processes them locally with no
     * further synchronization. This replaces the OpenMP dynamic
     * scheduler, which on Apple's libomp was causing lock contention
     * that stalled threads.
     */
    unsigned int counter = 0;

    #pragma omp parallel num_threads(threads)
    {
        while (1)
        {
            /* Atomically grab the next starting index for this thread */
            unsigned int start;
            #pragma omp atomic capture
            { start = counter; counter += grain; }

            if (start >= total) break;

            /* Clamp the end of this chunk to the total work */
            unsigned int end = start + grain;
            if (end > total) end = total;

            /* Process the chunk — no synchronization needed inside */
            for (unsigned int k = start; k < end; k++)
            {
                unsigned int i, j;
                decode_index(k, n, &i, &j);

                double temp        = mat->ptr[i * n + j];
                mat->ptr[i * n + j] = mat->ptr[j * n + i];
                mat->ptr[j * n + i] = temp;
            }
        }
    }
}