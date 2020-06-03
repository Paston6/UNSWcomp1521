// Copy input to output
// COMP1521 18s1
// z5142063 XingXing
#include <stdlib.h>
#include <stdio.h>

void copy(FILE *, FILE *);
int main(int argc, char *argv[]){
    copy(stdin,stdout);
    return EXIT_SUCCESS;
}

// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode

void copy(FILE *input, FILE *output){
    char c;
    while (fscanf(input,"%c",&c)!= EOF){
        fprintf(output,"%c", c);
    }
}
/*
fscanf(FILE *restrict stream, const char *restrict format, ...);
fprintf(FILE * restrict stream, const char * restrict format, ...);
fgetc(FILE *stream);
fputc(int c, FILE *stream);
fputs(const char *restrict s, FILE *restrict stream);
fgets(char * restrict str, int size, FILE * restrict stream);
*/