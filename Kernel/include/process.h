#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>

typedef enum {
    Ready = 0,
    Blocked,
    Running
} ProcessState;

typedef struct {
    uint8_t name[20];
    uint8_t ppid;
    uint8_t id;
    uint8_t priority;
    void (*function)(void);
    void *stack;
    void *base_pointer;
    uint8_t foreground;
    ProcessState state;
} Process;

void createProcess(uint8_t *name, uint8_t priority, void (*function)(void), uint8_t foreground, uint8_t ppid);

void killProcess(uint8_t id);

void modifyPriority(uint8_t id, uint8_t priority);

void blockProcess(uint8_t id);

void readyProcess(uint8_t id);

void runProcess(uint8_t id);

void printProcess(Process* process);

#endif