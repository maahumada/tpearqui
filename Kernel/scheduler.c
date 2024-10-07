#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <process.h>
#include <init.h>
#include <stdint.h>

typedef struct Node {
    Process* data;
    Node* next;
} Node;

typedef Node* ListProcess;

typedef struct {
    ListProcess current;
    uint64_t size;
} List;

List scheduler;

void initializeScheduler(void (*initFunction)(void)){
    scheduler.size = 0;
    create_process("init", 1, initFunction, 0);
}

void addSchedulerProcess(Process* process){
    void* p;
    mm_malloc(sizeof(Node), &p);
    ListProcess newProcess = p;
    newProcess->data = process;
    if(scheduler.current == NULL){ // First process is self-looped and set as current
        newProcess->next = newProcess;
        scheduler.current = newProcess;
    } else { // Other processes are inserted right after the current scheduler process
        newProcess->next = scheduler.current->next;
        scheduler.current->next = newProcess;
    }
    scheduler.size++;
}

// void list_processes() {
//     printf("Processes:\n");
//     for (int i = 0; i < num_processes; i++) {
//         Process *process = &processes[i];
//         printf("Name: %s\n", process->name);
//         printf("ID: %d\n", process->id);
//         printf("Priority: %d\n", process->priority);
//         printf("Stack: %p\n", process->stack);
//         printf("Base Pointer: %p\n", process->base_pointer);
//         printf("Foreground: %d\n", process->foreground);
//         // Print any other necessary variables here
//         printf("\n");
//     }
// }