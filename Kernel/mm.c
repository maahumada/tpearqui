#include <mm.h>

#define BLOCK_SIZE 256
#define BLOCK_COUNT 1024

typedef struct MemoryManagerCDT {
    uint64_t current;
    void* freeBlocks[BLOCK_COUNT];
} MemoryManagerCDT;

MemoryManagerCDT mm;

void initializeMemoryManager(void* start){
  mm.current = 0;
  for(int i = 0; i < BLOCK_COUNT; i++)
    mm.freeBlocks[i] = start + i * BLOCK_SIZE;
}

void *mm_malloc(uint64_t size){
	if(size > BLOCK_SIZE || mm.current >= BLOCK_COUNT - 1)
		return NULL;
  return mm.freeBlocks[(mm.current)++];
}    

void mm_free(void *p){
  mm.freeBlocks[--(mm.current)] = p; 
}