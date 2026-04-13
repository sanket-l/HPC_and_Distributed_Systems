#include <stdlib.h>
#include <math.h>

#define MAX_M 2048
#define MAX_CYCLES 1000000
#define EPS 1e-5

static inline int uniform_rand(int m) {
    return rand() % m;
}

double normal_rand(double mean, double stddev) {
    double u1 = ((double) rand() + 1) / ((double) RAND_MAX + 1);
    double u2 = ((double) rand() + 1) / ((double) RAND_MAX + 1);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return z0 * stddev + mean;
}

void simulate(double *avg_wait, int avg_wait_l, int procs, char dist) {

    for (int m = 1; m <= avg_wait_l; m++) {

        int *request = (int*) malloc(sizeof(int) * procs);
        int *granted = (int*) calloc(procs, sizeof(int));
        int *waiting = (int*) calloc(procs, sizeof(int));
        int *mem_busy = (int*) calloc(m, sizeof(int));

        int *mu = (int*) malloc(sizeof(int) * procs);
        if (dist == 'n') {
            for (int i = 0; i < procs; i++) {
                mu[i] = uniform_rand(m);
            }
        }

        int start = 0;
        double prev_avg = 0.0, curr_avg = 0.0;

        for (int cycle = 1; cycle <= MAX_CYCLES; cycle++) {

            for (int i = 0; i < m; i++) mem_busy[i] = 0;

            for (int i = 0; i < procs; i++) {
                if (waiting[i] == 0) {
                    if (dist == 'u') {
                        request[i] = uniform_rand(m);
                    } else {
                        double x = normal_rand(mu[i], m / 6.0);
                        int val = ((int) round(x)) % m;
                        if (val < 0) val += m;
                        request[i] = val;
                    }
                }
            }

            int granted_this_cycle = 0;
            int first_denied = -1;

            for (int k = 0; k < procs; k++) {
                int i = (start + k) % procs;
                int req = request[i];

                if (!mem_busy[req]) {
                    mem_busy[req] = 1;
                    granted[i]++;
                    waiting[i] = 0;
                    granted_this_cycle++;
                } else {
                    waiting[i]++;
                    if (first_denied == -1) {
                        first_denied = i;
                    }
                }
            }

            if (first_denied != -1)
                start = first_denied;

            double total = 0.0;
            for (int i = 0; i < procs; i++) {
                if (granted[i] > 0)
                    total += (double) cycle / granted[i];
            }
            curr_avg = total / procs;

            if (cycle > 1) {
                double diff = fabs(1.0 - prev_avg / curr_avg);
                if (diff < EPS) break;
            }

            prev_avg = curr_avg;
        }

        avg_wait[m - 1] = curr_avg;

        free(request);
        free(granted);
        free(waiting);
        free(mem_busy);
        free(mu);
    }
}
