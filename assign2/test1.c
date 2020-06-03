// COMP1521 18s1 Assignment 2
// Implementation of heap management system
// XING XING (PASTON)
// Z5142063

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myHeap.h"

// minimum total space for heap
#define MIN_HEAP  4096
// minimum amount of space for a free Chunk (excludes Header)
#define MIN_CHUNK 32

#define ALLOC     0x55555555
#define FREE      0xAAAAAAAA

typedef unsigned int uint;   // counters, bit-strings, ...

typedef void *Addr;          // addresses

typedef struct {             // headers for Chunks
    uint  status;             // status (ALLOC or FREE)
    uint  size;               // #bytes, including header
} Header;

static Addr  heapMem;        // space allocated for Heap
static int   heapSize;       // number of bytes in heapMem
static Addr *freeList;       // array of pointers to free chunks
static int   freeElems;      // number of elements in freeList[]
static int   nFree;          // number of free chunks

int checkhead(void *block);
void sorting();
void united(void *freeaddr);

// initialise heap
int initHeap(int size){
    // round up size to mult4
    if (size <= MIN_HEAP) {
        size = MIN_HEAP;
    } else {
        if((size % 4) != 0){
            size = (4 - (size % 4)) + size;
        }
    }
    
    // check and malloc space
    heapMem = malloc(size * sizeof(char));
    if (heapMem == NULL) {
        return -1;
    }
    memset(heapMem, 0, size);
    heapSize = size;
    
    // make the first header
    Header *first = (Header* )heapMem;
    first->size = heapSize;
    first->status = FREE;
    
    // total max free elements
    freeElems = heapSize/MIN_CHUNK;
    
    // check and malloc space for list
    freeList = malloc(freeElems * sizeof(char));
    if (freeList == NULL) {
        return -1;
    }
    memset(freeList, 0, freeElems);
    
    //create the first element
    freeList[0] = heapMem;
    nFree = 1;
    return 0;
}

// clean heap
void freeHeap(){
    free(heapMem);
    free(freeList);
}

// allocate a chunk of memory
void *myMalloc(int size){
    // if invalid return
    if(size < 1 || nFree <= 0){
        return NULL;
    }
    
    // round up size
    if(size % 4 != 0){
        size = size + (4 - (size % 4));
    }
    
    // calculate range of chunk
    int hsize = sizeof(Header);
    int rnglo = hsize+size;
    int rnghi = hsize + size + MIN_CHUNK;
    
    // for record satisfy free chunk
    Header *curr;
    int minsize = 0;
    int minindex = 0;
    int i = 0;
    
    // find satisfy free chunk and record
    while(i < nFree){
        curr = (Header *)freeList[i];
        // if size enough and lower than record
        if((curr->size >= rnglo) && (curr->size <= minsize)){
            minsize = curr->size;
            minindex = i;
        }
        // if size is the first enough item
        if((curr->size >= rnglo) && (minsize == 0)){
            minsize = curr->size;
            minindex = i;
        }
        i++;
    }
    // adjust the array
    if(minsize < rnglo){
        // if not found
        return NULL;
    }else if(minsize >= rnghi){
        // if its bigger than rnglo
        Header *min = (Header *) freeList[minindex];
        min->size = rnglo;
        min->status = ALLOC;
        Addr new = (Addr)((char *)freeList[minindex] + hsize + size);
        Header *newfree =  (Header *) new;
        newfree->size = minsize - hsize - size;
        newfree->status = FREE;
        freeList[minindex] = new;
    }else {
        // if its satisfied
        Header *min = (Header *) freeList[minindex];
        min->status = ALLOC;
        while(minindex < nFree){
            freeList[minindex] = freeList[minindex+1];
            minindex++;
        }
        nFree = nFree - 1;
    }
    // return free memory
    return (Addr)((char *)min+hsize);
}



void myFree(void *block){
     //= check is head Addr
     if(checkhead(block) == 0){
     fprintf(stderr, "Attempt to free unallocated chunk\n");
     exit(1);
     return;
     }
     //= the start and top Addr
     Addr top = (Addr)((char* )heapMem + heapSize);
     Header *freehead = (Header* )block;
     Addr freeaddr = block;
     int freesize = freehead->size;
     
     //= General checking=================================================
     if (block >= top || block < heapMem || block == NULL || nFree == 0) {
     fprintf(stderr, "Attempt to free unallocated chunk\n");
     exit(1);
     return;
     }
     if (freehead->status != ALLOC) {
     fprintf(stderr, "Attempt to free unallocated chunk\n");
     exit(1);
     return;
     }
     //= Checking finish==================================================
     
     //= free header
     memset(block, 0, (freesize - sizeof(Header)));
     freehead->status = FREE;
     
     freeList[nFree] = freeaddr;
     sorting();
     united(freeaddr);
}

// convert pointer to offset in heapMem
int  heapOffset(void *p)
{
    Addr heapTop = (Addr)((char *)heapMem + heapSize);
    if (p == NULL || p < heapMem || p >= heapTop)
        return -1;
    else
        return p - heapMem;
}

// dump contents of heap (for testing/debugging)
void dumpHeap(){
    Addr    curr;
    Header *chunk;
    Addr    endHeap = (Addr)((char *)heapMem + heapSize);
    int     onRow = 0;
    curr = heapMem;
    while (curr < endHeap) {
        char stat;
        chunk = (Header *)curr;
        switch (chunk->status) {
            case FREE:  stat = 'F'; break;
            case ALLOC: stat = 'A'; break;
            default:    fprintf(stderr,"Corrupted heap %08x\n",chunk->status); exit(1); break;
        }
        printf("+%05d (%c,%5d) ", heapOffset(curr), stat, chunk->size);
        onRow++;
        if (onRow%5 == 0) printf("\n");
        curr = (Addr)((char *)curr + chunk->size);
    }
    if (onRow > 0) printf("\n");
}

int checkhead(void *block){
    Addr srch = heapMem;
    Addr heaptop = (Addr)((char *)heapMem + heapSize);
    Header *curr = (Header* )srch;
    
    int found = 0;
    
    while(srch != heaptop){
        curr = (Header* )srch;
        if (srch == block){
            found = 1;
        }
        srch = (Addr)((char *)srch + curr->size);
    }
    return found;
}

void sorting(){
    Addr tmp;
    for (int i = 0; i < nFree; i++){
        for (int j = i + 1; j < nFree; j++){
            if (freeList[i] > freeList[j]){
                tmp =  freeList[i];
                freeList[i] = freeList[j];
                freeList[j] = tmp;
            }
        }
    }
}

void united(void *freeaddr){
    Addr top = (Addr)((char* )heapMem + heapSize);
    if (freeaddr == heapMem) {
        // at first
        int i = 0;
        Header *tmp = freeList[0];
        Header *next = freeList[1];
        if ((Addr)((char *)freeList[0] + tmp->size) == freeList[1]) {
            i = 1;
            memset(freeList[1] , 0 , next->size);
            tmp->size = tmp->size + next->size;
            while (i + 1 < nFree) {
                freeList[i] = freeList[i + 1];
                i++;
            }
            nFree--;
        }
    } else if (freeaddr == top){
        // at last
        int i = 0;
        i = nFree - 2;
        Header *tmp = freeList[i];
        Header *next = freeList[i + 1];
        if ((Addr)((char *)freeList[i] + tmp->size) == freeList[i + 1]) {
            tmp->size = tmp->size + next->size;
            memset(freeList[i + 1], 0, next->size);
        }
        nFree--;
    } else {
        int i = 1; // to nfree-2
        while (freeList[i] != freeaddr) {
            i++;
        }
        Header *left = freeList[i - 1];
        Header *next = freeList[i];
        Header *right = freeList[i + 1];
        if ((Addr)((char *)freeList[i-1] + left->size) == freeList[i]) {
            left->size = left->size + next->size;
            memset(freeList[i], 0, next->size);
            int k = i;
            while ((k+1)  < nFree) {
                freeList[k] = freeList[k + 1];
                k++;
            }
            nFree--;
        }
        if ((Addr)((char *)freeList[i-1] + left->size) == freeList[i]) {
            left->size = left->size + right->size;
            memset(freeList[i], 0, next->size);
            int j = i;
            while ((j + 1)  < nFree) {
                freeList[j] = freeList[j + 1];
                j++;
            }
            nFree--;
        }
    }
}