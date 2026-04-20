#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "primes.h"
#include "util.h"

#define MAX 10000000UL

void
print_help(char *prog_name)
{
    printf("Find prime numbers up to a maximum value.\n\n"
           "USAGE:\n"
           "  %s <max> <met> <t> <verb>\n\n", prog_name);
    printf("ARGUMENTS:\n"
           "  max   : Maximum number to test.\n"
           "  met   : Method: Sequential : 's'\n"
           "                    Parallel : 'p'\n"
           "  t     : Number of threads to use. Ignored when met='s'\n"
           "  verb  : 0=prints only execution time.\n"
           "          1=prints also the numbers found.\n");
}

int main(int argc, char **argv){
    //help message
    if(argc != 5){
        print_help(argv[0]);
        exit(1);
    }

    //get the command line arguments
    unsigned int max, met, t, verb;
    max=atoi(argv[1]);
    met=*argv[2];
    t=atoi(argv[3]);
    verb=atoi(argv[4]);
    if(MAX < max){
        printf("You can search primes only up to %lu.\n", MAX);
        exit(1);
    }
    if(met == 'p' && t < 1){
        printf("The number of threads 't' (%u) must be >= 1\n", t);
        exit(1);
    }

    //run and measure time taken
    double end, start;
    if(met == 's'){
        start = stopwatch();
        primes_seq(max, verb);
        end = stopwatch();
    }else if(met == 'p'){
        start = stopwatch();
        primes_par(max, t, verb);
        end = stopwatch();
    }else{
        printf("Error: incorrect value of 'met' (%c).\n", met);
        exit(1);
    }

    //print execution time
    printf("%.8f\n", end - start);
    
    return 0;
}
