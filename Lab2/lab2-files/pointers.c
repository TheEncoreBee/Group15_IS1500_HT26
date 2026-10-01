/*
 pointers.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int arr1[80];
int *list1 = arr1;
int arr2[80];
int *list2=arr2;
int counter=0;
int *count=&counter;

char* copycodes(char* a0, int* a1, int* a2){
    while(*a0!='\0'){
        *a1=(int)*a0;
        ++a0;//gets moved 1 byte due to char
        ++a1; //gets moved 4 bytes due to integers
        *a2+=1;
    }
    return a0;
}

char* work(){
    copycodes(text1, list1, count);
    copycodes(text2, list2, count);
    return text2;
}

void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n",
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));

}

int main(void){


    work();
    printf("\nlist1: ");
    printlist(list1);
    printf("\nlist2: ");
    printlist(list2);
    printf("\nCount = %d\n", counter);

    endian_proof((char*) &counter);
}
