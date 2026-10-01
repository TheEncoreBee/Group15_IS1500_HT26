#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 6
int counter = 0;

void print_number ( int n ) {
   printf ( "%10d " , n ) ;
   ++counter;
   if (counter==COLUMNS){
       (printf("\n"));
       counter =0;
   }
}

void print_sieves ( int n ){
//printf("wtf");
//printf("wtf222");
//arr
char arr[(n-1)];//If we made in unitialized, some elements could have been marked '-' before we began.
for(int i=0; i<(n-1); i++){
    arr[i]='?';
    //printf("arr[i] is %c /n", arr[i]);
}
int p;

//We are going to mark the multiples of p with '-'.
for(int i=0; i<(n-1);i++){
    if(arr[i]!='-'){

        p=(i+2);
        //printf("i is %d", i);
        //printf("p is %d", p);
        print_number(p);
        for(int j=i+p; j<(n-1); j=j+p){
            arr[j]='-';
        }
    }
}
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){

    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
    else
    printf("Please state an integer number.\n");
  return 0;
}
