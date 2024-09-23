#ifndef __MM_H_

#define __MM_H_

#include <stdint.h>
#include <stdlib.h>

void initializeMemoryManager(void* start);

void *mm_malloc(uint64_t size, void** p);

void mm_free(void *p);

#endif