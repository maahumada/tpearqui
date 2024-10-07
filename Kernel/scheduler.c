#include <mm.h>
#include <init.h>
#include <scheduler.h>

typedef struct Node {
    Process* data;
    Node* next;
} Node;

typedef Node* ListProcess;

typedef struct {
    ListProcess current;
    uint64_t size;
    ListProcess first;
} List;

List scheduler;

void initializeScheduler(void (*initFunction)(void)){
    scheduler.size = 1;
    createProcess("init", 1, initFunction, 0, -1);
}

void addSchedulerProcess(Process* process){
    void* p;
    ListProcess newProcess = mm_malloc(sizeof(Node), &p);
    newProcess->data = process;
    if(scheduler.current == NULL){ // First process is self-looped and set as current
        newProcess->next = newProcess;
        scheduler.current = newProcess;
        scheduler.first = newProcess;
    } else { // Other processes are inserted right after the current scheduler process
        newProcess->next = scheduler.current->next;
        scheduler.current->next = newProcess;
    }
    scheduler.size++;
}

void removeNSchedulerProcesses(Process* process, uint8_t n){
    if(scheduler.size < 2){
        return;
    }
    uint8_t removed = 0;
    ListProcess first = scheduler.first;
    ListProcess previous = scheduler.first;
    ListProcess l = scheduler.first->next;
    while(l != first && removed < n){
        ListProcess next = l->next;
        if(l->data == process){
            previous->next = next;
            mm_free(l);
            scheduler.size--;
            removed++;
        }
        l = next;
    }
}

void removeSchedulerProcess(Process* process){
    removeNSchedulerProcesses(process, process->priority);
}

void listProcesses(){
    ListProcess first = scheduler.first;
    ListProcess l = scheduler.first->next;
    printProcess(first);
    while(l != first){
        printProcess(l->data);
        l = l->next;
    }
}

void switchProcess(){
    scheduler.current->data->state = Ready;
    ListProcess l = scheduler.current;
    while(1){ // Take the next process in ready status
        if(l->data->state != Ready){
            l->data->state = Running;
            scheduler.current = l;
            return;
        }
        l = l->next;
    }
}

void getPid(uint8_t* pid){
    *pid = scheduler.current->data->id;
}

void changeSchedulerProcessPriority(Process* process, uint8_t priority){
    uint8_t diff = process->priority - priority;
    if(diff > 0){
        // Delete diff times
        removeNSchedulerProcesses(process, diff);
    } else {
        // Add diff times
        for(int i = 0; i < diff; i++){
            addSchedulerProcess(process);
        }
    }
}