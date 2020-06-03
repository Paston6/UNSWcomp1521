// COMP1521 18s1 Assignment 2
// Implementation of heap management system
// XING XING
// (Z5142063)
// sxr0203@163.com

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myHeap.h"

// Targetimum total space for heap
#define MIN_HEAP  4096
// Targetimum amount of space for a free Chunk (excludes Header)
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

void CombineFree();
void insertSort();

// initialise heap
int initHeap(int size){
    // check and round up size
    if (size < MIN_HEAP) {
        size = MIN_HEAP;
    } else {
        if((size % 4) != 0){
            size = (4 - (size % 4)) + size;
        }
    }
    
    // malloc heap space and
    heapMem = malloc(size * sizeof(char));
    if (heapMem == NULL) return -1;
    memset(heapMem, 0, size);
    
    // initial heap information
    heapSize = size;
    freeElems = (heapSize/MIN_CHUNK);
    
    // initial heap header
    Header *first = (Header* )heapMem;
    first->size = heapSize;
    first->status = FREE;
    
    // initial freeList[nFree]
    freeList = malloc(freeElems * sizeof(char));
    if (freeList == NULL) return -1;
    memset(freeList, 0, freeElems);
    
    //initial nFree = 1
    freeList[0] = heapMem;
    nFree = 1;
    return 0;
}

// clean heap
void freeHeap(){
    free(heapMem);
    free(freeList);
}

// allocate a chunk of memoryyy
void *myMalloc(int size){
    // check valid nFree and size
    if(size < 1 || nFree < 1){
        return NULL;
    }
    
    if(nFree > freeElems || size > heapSize){
        return NULL;
    }
    
    // round up size
    if(size % 4 != 0){
        size = size + (4 - (size % 4));
    }
    
    // find satisfy range
    int low = sizeof(Header)+size;
    int high = sizeof(Header) + size + MIN_CHUNK;
    
    // for record the imformation of Target
    Header *curr;
    int Targetsize = 0;
    int Targetindex = 0;
    int i = 0;
    
    // find the Targetium satisfy Header
    while(i < nFree){
        curr = (Header *)freeList[i];
        if(curr->size >= low && (curr->size <= Targetsize || Targetsize == 0)){
            Targetsize = curr->size;
            Targetindex = i;
        }
        i++;
    }
    Header *Target = (Header *) freeList[Targetindex];
    //delete Target from freeList
    if(Targetsize < low){
        return NULL;
    } else if(Targetsize >= high){
        Target->size = low;
        Target->status = ALLOC;
        Addr new = (Addr)((char *)freeList[Targetindex] + sizeof(Header) + size);
        Header *newfree =  (Header *) new;
        newfree->size = Targetsize - sizeof(Header) - size;
        newfree->status = FREE;
        freeList[Targetindex] = new;
    }else {
        Target->status = ALLOC;
        while(Targetindex < nFree){
            freeList[Targetindex] = freeList[Targetindex+1];
            Targetindex++;
        }
        nFree = nFree - 1;
    }
    Addr p = (Addr)((char *)Target+sizeof(Header));
    return p;
    
}

// free a chunk of memory
void myFree(void *block){
    Addr top = (Addr)((char *)heapMem + heapSize);
    Addr freehead = heapMem;
    Header *curr = (Header*)freehead;
    
    while(freehead < top){
        curr = (Header *)freehead;
        if(block == (Addr)((char *)freehead + sizeof(Header))){
            break;
        }
        freehead = (Addr)((char *)freehead + curr->size);
    }
    
    if(block >= top || block < heapMem){
        fprintf(stderr,"Attempt to free unallocated chunk\n");
        exit(1);
    }
    
    // if not found
    if(freehead == top){
        fprintf(stderr,"Attempt to free unallocated chunk\n");
        exit(1);
    } else if(curr->status == FREE){
        fprintf(stderr,"Attempt to free unallocated chunk\n");
        exit(1);
    } else {
        curr->status = FREE;
        memset(block, 0, (curr->size - sizeof(Header)));
    }
    
    nFree++;
    freeList[nFree - 1] = freehead;
    
    CombineFree();
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
void dumpHeap()
{
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

// insertionsort freeList
void insertSort(){
    int a = 1;
    int b = 0;
    Addr tmp = freeList[a];
    while(a < nFree){
        tmp = freeList[a];
        b = a - 1;
        while ((freeList[b] > tmp) && (b >= 0)){
            freeList[b + 1] = freeList[b];
            b--;
        }
        freeList[b + 1] = tmp;
        a++;
    }
}

// merge every neighbour free chunk
void CombineFree(){
    // sort the freeList in decending
    insertSort();
    Addr tmpaddr;
    int j = 0;
    
    // combine and free the right chunk
    while(j < (nFree - 1)){
        Header *LeftAddr = (Header*)freeList[j];
        Header *RightAddr = (Header*)freeList[j + 1];
        tmpaddr = (Addr)((char* )freeList[j] + LeftAddr->size);
        
        // if they are neighbour
        if(freeList[j + 1] == tmpaddr){
            LeftAddr->size += RightAddr->size;
            LeftAddr->status = FREE;
            memset(freeList[j + 1], 0, RightAddr->size);
            int k = j + 1;
            
            // shift all items left
            while(k + 1 < nFree){
                freeList[k] = freeList[k + 1];
                k++;
            }
            
            // scratch one off list
            nFree--;
        } else {
            j++;
        }
    }
}