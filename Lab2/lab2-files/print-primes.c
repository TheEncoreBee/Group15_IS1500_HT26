/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
int counter = 0;

int is_prime(int n){
    if (n<=1){
        return 0;
    }

    else{
        // We only need to check for divisors up to square root of n, cause after that point the two factors that can make up
        // number change place.
        for(int i=2; i*i<=n; i++){
            if(n%i==0){
                return 0;
            }

        }
    }
    return 1;
}

void print_number ( int n ) {
   printf ( "%10d " , n ) ;
   ++counter;
   if (counter==COLUMNS){
       (printf("\n"));
       counter =0;
   }
}

void print_primes(int n){
    // Should print out all prime numbers less than 'n'
    // with the following formatting. Note that
    // the number of columns is stated in the define
    // COLUMNS
    for(int i=0; i<=n; i++){
        if(is_prime(i)==1){
            print_number(i);
        }
    }
}



// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
  else
    printf("Please state an integer number.\n");
  return 0;
}
