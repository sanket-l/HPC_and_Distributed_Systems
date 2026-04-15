#include <stdlib.h>
#include <math.h>

#define MAX_M 2048
#define MAX_CYCLES 1000000
#define EPS 1e-5

static inline int uniform_rand(int m) {
    return rand() % m;
}

// Generate random number with normal distribution using Box-Muller method
double normal_rand(double mean, double stddev) {
    double u1 = ((double) rand() + 1) / ((double) RAND_MAX + 1);
    double u2 = ((double) rand() + 1) / ((double) RAND_MAX + 1);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return z0 * stddev + mean;
}

// Simulate memory request contention for different numbers of memory modules
// Calculates average wait time for each number of modules from 1 to avg_wait_l
void simulate(double *avg_wait, int avg_wait_l, int procs, char dist) {

    for (int m = 1; m <= avg_wait_l; m++) {

        // Allocate arrays for processor requests, grants, and wait states
        int *request = (int*) malloc(sizeof(int) * procs);
        int *granted = (int*) calloc(procs, sizeof(int));
        int *waiting = (int*) calloc(procs, sizeof(int));
        int *mem_busy = (int*) calloc(m, sizeof(int));

        // For normal distribution, pre-assign each processor a mean memory address
        int *mu = (int*) malloc(sizeof(int) * procs);
        if (dist == 'n') {
            for (int i = 0; i < procs; i++) {
                mu[i] = uniform_rand(m);
            }
        }

        int start = 0;
        double prev_avg = 0.0, curr_avg = 0.0;

        // Run simulation cycles until average wait time converges
        for (int cycle = 1; cycle <= MAX_CYCLES; cycle++) {

            // Reset memory modules (all idle at start of cycle)
            for (int i = 0; i < m; i++) mem_busy[i] = 0;

            // Generate memory requests from each processor
            for (int i = 0; i < procs; i++) {
                if (waiting[i] == 0) {
                    if (dist == 'u') {
                        request[i] = uniform_rand(m);
                    } else {
                        double x = normal_rand(mu[i], m / 6.0);
                        int val = (int) round(x);
                        if (val < 0)
                            // val = (val % m + m) % m;
                            val = 0;
                        else if (val >= m)
                            // val = val % m;
                            val = m - 1;
                        request[i] = val;
                    }
                }
            }

            // Process requests in a round-robin fashion starting from first denied processor
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

            // Calculate average wait time as cycles per granted request
            double total = 0.0;
            for (int i = 0; i < procs; i++) {
                if (granted[i] > 0)
                    total += (double) cycle / granted[i];
            }
            curr_avg = total / procs;

            // Check for convergence: stop if average stabilized
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
