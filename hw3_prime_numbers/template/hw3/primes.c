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

void primes_par(unsigned int max, unsigned int threads, unsigned int verb)
{
    int count = 0;

    #pragma omp parallel num_threads(threads)
    {
        // Each thread will atomically capture the current value of count and then increment it
        int tmp = 0;

        // Loop until we reach the maximum number (1e7)
        while(tmp < 1e7){

            // OMP atomic capture ensures that the value of count is captured 
            // and incremented atomically, preventing race conditions
            #pragma omp atomic capture
            tmp = count++;
            
            // Check if the captured value is a prime number and print it if verbose mode is enabled
            if(is_prime(tmp) && verb)
                printf("%d\n", tmp);
        }
    }

    return;
}
