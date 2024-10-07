#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>

typedef struct {
    uint8_t name[20];
    uint8_t id;
    uint8_t priority;
    void (*function)(void);
    void *stack;
    void *base_pointer;
    uint8_t foreground;
} Process;

void create_process(uint8_t *name, uint8_t priority, void (*function)(void), uint8_t foreground);

void kill_process(uint8_t id);

void modify_priority(uint8_t id, uint8_t priority);

void block_process(uint8_t id);

void unblock_process(uint8_t id);

#endif