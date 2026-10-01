/*
 prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>

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
int main(void){
  printf("%d\n", is_prime(11));  // 11 is a prime.      Should print 1.
  printf("%d\n", is_prime(383)); // 383 is a prime.     Should print 1.
  printf("%d\n", is_prime(987)); // 987 is not a prime. Should print 0.
}
