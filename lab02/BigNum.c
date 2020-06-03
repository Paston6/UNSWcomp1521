// BigNum.h ... LARGE positive integer values

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "BigNum.h"

// Initialise a BigNum to N bytes, all zero
void initBigNum(BigNum *n, int Nbytes){
    n->nbytes = Nbytes;
    n->bytes = calloc(Nbytes,sizeof(int));
    int i = 0;
    while (i < Nbytes) {
        n->bytes[i] = 0;
        i++;
    }
    return;
}





// Add two BigNums and store result in a third BigNum
void addBigNums(BigNum n, BigNum m, BigNum *res){
    int len = (n->nbytes >= m->nbytes) ? n->nbytes : m->nbytes;
    int sho = (n->nbytes <= m->nbytes) ? n->nbytes : m->nbytes;
    int i = 0;
    //find the maxim size and realloc
    if(res->nbytes < len + 1){
        size = len + 1;
        initBigNum(*res, size);
    }
    while(i < size){
        if ((n->bytes[i] + m->bytes[i]) >= 10) {
            sum = n->bytes[i] + m->bytes[i];
            sum = sum - 10;
            res->bytes[i] = sum;
        }
    }
    return;
}
















// Set the value of a BigNum from a string of digits
// Returns 1 if it *was* a string of digits, 0 otherwise
int scanBigNum(char *s, BigNum *n)
{
   // TODO
   return 1;
}

// Display a BigNum in decimal format
void showBigNum(BigNum n)
{
   // TODO
   return;
}

