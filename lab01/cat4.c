// Copy input to output
// COMP1521 18s1
// XING XING Z5142063
#include <stdlib.h>
#include <stdio.h>
void copy(FILE *, FILE *);
int main(int argc, char *argv[]){
    if (argc == 1) {
        copy(stdin, stdout);
    } else {
        // for each command line argument
        int i;
        for (i = 0; i < argc; i++) {
            FILE *input = fopen("argv[i]", "r");
            if(argv[i] == NULL){
                printf("Can't read %s",argv[i]);
            } else {
                copy(stdin,stdout);
                fclose(input);
            }
        }
    }
    return EXIT_SUCCESS;
}
// Copy contents of input to output, char-by-char
// Assumes both files open in appropriate mode
void copy(FILE *input, FILE *output){
    char line[BUFSIZ];
    while(fgets(line,BUFSIZ,input) != NULL){
        fputs(line,output);
    }
}