#include <stdio.h>
#include <math.h>
#include <omp.h>


/**
 * Checks if a given integer is a prime number.
 * @param number The integer to check.
 * @return 1 if the number is prime, 0 otherwise.
 */
int is_prime(int number)
{
    // Numbers less than or equal to 1 are not prime by definition
    if (number <= 1) {
        return 0;
    }
    // 2 is the only even prime number
    if (number == 2) {
        return 1;
    }
    
    // All even numbers greater than 2 are not prime
    if (number % 2 == 0) {
        return 0;
    }

    // Check for odd divisors from 3 up to the square root of the number
    for (int i = 3; i <= sqrt(number); i += 2) {
        if (number % i == 0) {
            return 0; // Found a divisor, so it's not prime
        }
    }

    return 1; // No divisors found, it is a prime number
}

void primes_seq(unsigned int max, unsigned int verb)
{
    // Loop through numbers from 0 to max-1 and check if each number is prime
    for(int i=0; i<max; i++) {
        // If the number is prime and verbose mode is enabled, print it
        if(is_prime(i) && verb)
            printf("%d\n", i);
    }
    return;
}

int primes_par(unsigned int max, unsigned int threads, unsigned int verb)
{
    int count = 0;
    // omp_lock_t lock;
    // omp_init_lock(&lock);
    
#pragma omp parallel shared(count) num_threads(threads)
    {
        int tmp = 0;
        // Loop until we reach the maximum number (1e7)
        while(count <= 1e7){

            // Alternate way
            // OMP atomic capture ensures that the value of count is captured 
            // and incremented atomically, preventing race conditions
            #pragma omp atomic capture
            tmp = count++;

            // omp_set_lock(&lock); // start critical section
            // int tmp = count;
            // count++;
            // omp_unset_lock(&lock); // end critical section

            // Check if the captured value is a prime number and print it if verbose mode is enabled
            if(is_prime(tmp)){
                int isPrime = 1;
                isPrime = isPrime + 1;
            }
        }
    }
    // omp_destroy_lock(&lock);

    return 0;
}

int main() {
    
    for(int i = 0; i < 7; i++){
    	int threadCount = pow(2,i);
    	
    	printf("Thread = %d\n", threadCount);

    	double start = omp_get_wtime();
	    primes_par(1e7, 64, 0);
	    double end = omp_get_wtime();
	    
	    double totalTime = end - start;
	    printf("Done in %fs!\n\n", totalTime);    	
	}
    

    return 0;
}