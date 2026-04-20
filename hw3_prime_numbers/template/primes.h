/**
 ** Finds and print all prime numbers beween 1 and 'max'.
 ** If 'verb' != 0 then print the prime numbers as they
 ** are found.
 **/
extern void primes_seq(unsigned int max, unsigned int verb);

/**
 ** Finds and print all prime numbers beween 1 and 'max'
 ** utilizing 'threads' threads. If 'verb' != 0 then print
 ** the prime numbers as they are found.
 **/
extern void primes_par(unsigned int max, unsigned int threads,	\
		      unsigned int verb);
