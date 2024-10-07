#ifndef __BUDDY_H_

#define __BUDDY_H_

#include <stdint.h>
#include <stdlib.h>

void initializeBuddy(void* start);

void *buddyMalloc(uint64_t size, void** p);

void buddyFree(void *p);

#endif