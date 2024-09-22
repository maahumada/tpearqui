#ifndef __MM_H_

#define __MM_H_

#include <stdint.h>
#include <stdlib.h>

void initializeMemoryManager(void* start);

void *mm_malloc();

void mm_free(void *p);

#endif