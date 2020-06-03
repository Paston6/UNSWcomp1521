// make_a_float ... read in bit strings to build a float value
// COMP1521 Lab03 exercises
// Written by John Shepherd, August 2017
// Completed by ...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef uint32_t Word;

struct _float {
    // define bit_fields for sign, exp and frac
    // obviously they need to be larger than 1-bit each
    // and may need to be defined in a different order
    unsigned int frac: 23, exp: 8, sign: 1;
};


//        1        8                   23
//     +-----------------+----------------------------+
//     |sign|     exp    |            frac            |
//     +-----------------+----------------------------+
//
//                      *IEEE standard

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


int main(int argc, char **argv)
{
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

#define charToBinary(ch) (ch == '1')

#define FLOAT_LENGTH 32
#define SIGNAL_BEGIN 1
#define EXP_BEGIN 2
#define EXP_END 9
#define FRAC_BEGIN 10
#define FRAC_END 32

static char charBitValue(Word val, int index){
    // index range: 1 - 32
    // using the bitwise opreation to calculate
    // the value with specified index
    
    // for example:
    //      binaryNum num = 1001 1100 1100 1100;
    //      BINARY_LENGTH = 16
    // now we look for the second digit(index = 2) in binary form of num
    // first let:
    //      binaryNum i = 1;
    //      i = 0000 0000 0000 0001;
    // do left shifting for i:
    //      i <<= FLOAT_LENGTH - index (i <<= 14)
    //      i = 0100 0000 0000 0000;
    // do bitwise operation & between num and i
    //      num = 1001 1100 1100 1100;
    //        i = 0100 0000 0000 0000;
    //      num & i = 0000 0000 0000 0000 = 0
    // and return ((num & i) != 0) (equals to 0 in this example)
    // done
    
    int intBitValue = 0;
    intBitValue = (val & (1 << (FLOAT_LENGTH - index))) != 0;
    if(intBitValue == 1){
        return '1';
    }else{
        return '0';
    }
}

// convert three bit-strings (already checked)
// into the components of a struct _float
Union32 getBits(char *sign, char *exp, char *frac)
{
    Union32 new;
    
    // this line is just to keep gcc happy
    // delete it when you have implemented the function
    new.bits.sign = new.bits.exp = new.bits.frac = 0;
    
    // convert char *sign into a single bit in new.bits
    new.bits.sign = charToBinary(*sign);
    
    // convert char *exp into an 8-bit value in new.bit
    int cursor = 0;
    new.bits.exp = new.bits.exp + charToBinary(exp[cursor]);
    for(cursor = 1; cursor < 8; cursor++){
        // when we want to add a digit, do a left shift
        new.bits.exp <<= 1;
        new.bits.exp += charToBinary(exp[cursor]);
    }
    
    // convert char *frac into a 23-bit value in new.bits
    cursor = 0;
    new.bits.frac += charToBinary(frac[cursor]);
    for(cursor = 1; cursor < 23; cursor++){
        new.bits.frac <<= 1;
        new.bits.frac += charToBinary(frac[cursor]);
    }
    
    return new;
}

// convert a 32-bit bit-stringin val into a sequence
// of '0' and '1' characters in an array buf
// assume that buf has size > 32
// return a pointer to buf


char *showBits(Word val, char *buf)
{
    int cursor = 0;
    int i = 0;
    // record the sign part:
    buf[cursor++] = charBitValue(val, SIGNAL_BEGIN);
    
    // add a space:
    buf[cursor++] = ' ';
    
    // record the exp part:
    for(i = EXP_BEGIN; i <= EXP_END; i++){
        buf[cursor++] = charBitValue(val, i);
    }
    
    // add a space
    buf[cursor++] = ' ';
    
    // record the frac part:
    for(i = FRAC_BEGIN; i <= FRAC_END; i++){
        buf[cursor++] = charBitValue(val, i);
    }
    
    // add a '\0'
    buf[cursor] = '\0';
    
    
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
