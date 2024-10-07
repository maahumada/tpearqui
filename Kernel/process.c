#include <mm.h>
#include <process.h>
#include <scheduler.h>
#include <videoDriver.h>

#define MAX_PROCESSES 64
#define STACK_SIZE 256

#define OUTPUT_COLOR 0xFFFFFF

Process processes[MAX_PROCESSES] = {0};
int numProcesses = 0;

void createProcess(uint8_t *name, uint8_t priority, void (*function)(void), uint8_t foreground, uint8_t ppid) {
    if (numProcesses >= MAX_PROCESSES) {
        return;
    }

    Process *process = &processes[numProcesses++];
    snprintf(process->name, sizeof(process->name), "%s", name);
    process->id = numProcesses;
    process->ppid = ppid;
    process->priority = priority;
    process->function = function;
    process->stack = mm_malloc(STACK_SIZE, &process->stack);
    process->base_pointer = process->stack + STACK_SIZE;
    process->foreground = foreground;
    process->state = Ready;

    for(int i = 0; i < priority; i++){
        addSchedulerProcess(process);
    }
}

void killProcess(uint8_t id) {
    removeSchedulerProcess(&processes[id]);
    mm_free(processes[id].stack);
}

void modifyPriority(uint8_t id, uint8_t priority) {
    changeSchedulerProcessPriority(&processes[id], priority);
    processes[id].priority = priority;
}

void blockProcess(uint8_t id) {
    processes[id].state = Blocked;
}

void readyProcess(uint8_t id) {
    processes[id].state = Ready;
}

void runProcess(uint8_t id){
    processes[id].state = Running;
}

void printProcess(Process* process) {
    puts("Process:\n", OUTPUT_COLOR);
    puts("Name: ", OUTPUT_COLOR);
    puts(process->name, OUTPUT_COLOR);
    // puts("\nID: ", OUTPUT_COLOR);
    // puts(process->id, OUTPUT_COLOR);
    // puts("\nPriority: ", OUTPUT_COLOR);
    // puts(process->priority, OUTPUT_COLOR);
    // puts("\nStack: ", OUTPUT_COLOR);
    // puts(process->stack, OUTPUT_COLOR);
    // puts("\nBase Pointer: ", OUTPUT_COLOR);
    // puts(process->base_pointer, OUTPUT_COLOR);
    // puts("\nForeground: ", OUTPUT_COLOR);
    // puts(process->foreground, OUTPUT_COLOR);
    // puts("\nState: ", OUTPUT_COLOR);
    // switch(process->state){
    //     case Ready: 
    //         puts("Ready\n", OUTPUT_COLOR);
    //         break;
    //     case Blocked:
    //         puts("Blocked\n", OUTPUT_COLOR);
    //         break;
    //     case Running:
    //         puts("Running\n", OUTPUT_COLOR);
    //         break;
    // }
    putChar('\n', OUTPUT_COLOR);
    print();
}