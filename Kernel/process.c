#include <mm.h>
#include <process.h>

#define MAX_PROCESSES 64
#define STACK_SIZE 256

Process processes[MAX_PROCESSES] = {0};
int num_processes = 0;
int current_process = -1;

void create_process(uint8_t *name, uint8_t priority, void (*function)(void), uint8_t foreground) {
    if (num_processes >= MAX_PROCESSES) {
        //Hasta acá
        return;
    }

    Process *process = &processes[num_processes++];
    snprintf(process->name, sizeof(process->name), "%s", name);
    process->id = num_processes;
    process->priority = priority;
    process->function = function;
    process->stack = mm_malloc(STACK_SIZE, &process->stack);
    process->base_pointer = process->stack + STACK_SIZE;
    process->foreground = foreground;

    addSchedulerProcess(process);
}

void kill_process(uint8_t id) {
    mm_free(processes[id].stack);
}

void modify_priority(uint8_t id, uint8_t priority) {
    processes[id].priority = priority;
}

void block_process(uint8_t id) {
    // Implementar
}

void unblock_process(uint8_t id) {
    // Implementar
}