#include <stdio.h>
#include <math.h>
#include <omp.h>


/**
 * Checks if a given integer is a prime number.
 * @param number The integer to check.
 * @return 1 if the number is prime, 0 otherwise.
 */
int
is_prime(int number)
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

void
primes_seq(unsigned int max, unsigned int verb)
{
    for(int i=0; i<max; i++) {
        if(is_prime(i) && verb)
            printf("%d\n", i);
    }
    return;
}

void
primes_par(unsigned int max, unsigned int threads, unsigned int verb)
{
    int count = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);
    
#pragma omp parallel default(none) shared(max, verb, count, lock) num_threads(threads)
    {
        int tmp;
        while(1){
            omp_set_lock(&lock); // start critical section
            tmp = count;
            count++;
            omp_unset_lock(&lock); // end critical section

            if(max < tmp)
                break;

            if(is_prime(tmp) && verb)
                printf("%d\n", tmp);
        }
    }
    omp_destroy_lock(&lock);

    return;
}
