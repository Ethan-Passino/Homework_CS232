/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int isPrime(int number) {
    // Here we check between 2 and half the number and see if there are any divsors other than 1 and itself.
    // If there are divisors we return false (0).
    for(int i = 2; i <= number/2; i++) {
        if((number % i) == 0) {
            return 0;
        }
    }

    // If there are no divisors, it is prime, returns true (1).
    return 1;
}

int main(int argc, const char** argv){

    int lowerBound, upperBound;

    if(argc != 3){
        fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
        return -1;
    }
  
    lowerBound = atoi(argv[1]);
    upperBound = atoi(argv[2]);
  
    if(lowerBound < 1 || upperBound < 1){
        fprintf(stderr, "ERROR: both the lowerBound (%d) and the upperBound (%d)"
	          " must be positive.\n", lowerBound, upperBound);
        return -2;
    }
    for(int i = lowerBound; i <= upperBound; i++)
    {
        if(i == 1) {
            // 1 cannot be prime.
            continue;
        }
        /* TODO: fill in the code that outputs the prime numbers */
        /*   in the range [lowerBound,upperBound] in ascending order */
        // See above main function to see my isPrime code.
        if(isPrime(i)) { 
            printf("%d\n", i);
        }

    }

    return 0;
}
