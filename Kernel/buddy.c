#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MinBlockSize 16  
#define MaxBlockSize 256 
#define BucketCount 5    


static uint8_t nodeIsSplit[(1 << (BucketCount - 1)) / 8];

void initializeBuddy(void *memoryStart, uint64_t totalSize) {
    //Implmentar
}

void *buddyMalloc(uint64_t size, void** p) {
    //Implmentar
    return NULL;
}

void buddyFree(void *ptr) {
    //Implmentar
}
