
// COMP1521 Lab03 exercises
// Written by John Shepherd, August 2017
// Completed by XING XING (Z5142063)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef uint32_t Word;

struct _float {
    unsigned int frac: 23, exp: 8, sign: 1;
};

typedef struct _float Float32;

union _bits32 {
    float   fval;  // interpret the bits as a float
    Word    xval;  // interpret as a single 32-bit word
    Float32 bits;  // manipulate individual bits
};
typedef union _bits32 Union32;

void    checkArgs(int, char **);
Union32 getBits(char *, char *, char *);
char   *showBits(Word, char *);
int     justBits(char *, int);
static int ctb (char a);

int main(int argc, char **argv){
    union _bits32 u;
    char out[50];
    
    // here's a hint ...
    u.bits.sign = u.bits.exp = u.bits.frac = 0;
    
    // check command-line args (all strings of 0/1
    // kills program if args are bad
    checkArgs(argc, argv);
    
    // convert command-line args into components of
    // a Float32 inside a Union32, and return the union
    u = getBits(argv[1], argv[2], argv[3]);
    
    printf("bits : %s\n", showBits(u.xval,out));
    printf("float: %0.10f\n", u.fval);
    
    return 0;
}
//=================================================
// convert three bit-strings (already checked)
// into the components of a struct _float
//============================================done=
Union32 getBits(char *sign, char *exp, char *frac){
    Union32 new;
    
    // convert char *sign into a single bit in new.bits
    new.bits.sign = (*sign == '1');
    
    // convert char *exp into an 8-bit value in new.bit
    int i = 0;
    new.bits.exp = new.bits.exp + ctb(exp[i]);
    i++;
    while (i < 8) {
        new.bits.exp <<= 1;
        new.bits.exp = new.bits.exp + ctb(exp[i]);
        i++;
    }
    
    // convert char *frac into a 23-bit value in new.bits
    i = 0;
    new.bits.frac = new.bits.frac + ctb(frac[i]);
    i++;
    while (i < 23) {
        new.bits.frac <<= 1;
        new.bits.frac = new.bits.frac + ctb(frac[i]);
        i++;
    }
    return new;
}
//==================================================
// convert a 32-bit bit-stringin val into a sequence
// of '0' and '1' characters in an array buf
// assume that buf has size > 32
// return a pointer to buf
//==================================================
char *showBits(Word val, char *buf){
    int i = 0;
    Word sample = 1 << 31;
    //=========
    //sign part
    //=========
    buf[i] = ((val & sample) != 0) + '0';
    i++;
    buf[i] = ' ';
    i++;
    //i = 2;
    //1 31x0
    //========
    //exp part
    //========
    while (i < 10) {
        sample >>= 1;
        buf[i] = ((val & sample) != 0) + '0';
        i++;
        
    }
    //8x0 1 23x0
    //i = 10
    buf[i] = ' ';
    i++;
    // i = 11
    
    // frac part:
    while (i <= 34) {
        sample >>= 1;
        buf[i] = ((val & sample) != 0) + '0';
        i++;
    }
    buf[i] = '\0';
    
    
    return buf;
}

// checks command-line args
// need at least 3, and all must be strings of 0/1
// never returns if it finds a problem
void checkArgs(int argc, char **argv)
{
    if (argc < 3) {
        printf("Usage: %s Sign Exp Frac\n", argv[0]);
        exit(1);
    }
    if (!justBits(argv[1],1)) {
        printf("%s: invalid Sign: %s\n", argv[0], argv[1]);
        exit(1);
    }
    if (!justBits(argv[2],8)) {
        printf("%s: invalid Exp: %s\n", argv[0], argv[2]);
        exit(1);
    }
    if (!justBits(argv[3],23)) {
        printf("%s: invalid Frac: %s\n", argv[0], argv[3]);
        exit(1);
    }
    return;
}

// check whether a string is all 0/1 and of a given length
int justBits(char *str, int len)
{
    if (strlen(str) != len) return 0;
    
    while (*str != '\0') {
        if (*str != '0' && *str != '1') return 0;
        str++;
    }
    return 1;
}

//change a char to an int binary shit
static int ctb(char a){
    return (a == '1');
}