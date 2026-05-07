#include "util.h"

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

void mat_sqtrans_par(Mat *mat, unsigned int grain, unsigned int threads)
{
    unsigned int n = mat->n;

    unsigned int total = n * (n - 1) / 2;

#pragma omp parallel for num_threads(threads) schedule(dynamic, grain)
    for (unsigned int k = 0; k < total; k++)
    {

        unsigned int tmp = k;
        unsigned int i = 0;

        while (tmp >= (n - i - 1))
        {
            tmp -= (n - i - 1);
            i++;
        }

        unsigned int j = i + 1 + tmp;

        double temp = mat->ptr[i * n + j];
        mat->ptr[i * n + j] = mat->ptr[j * n + i];
        mat->ptr[j * n + i] = temp;
    }
}