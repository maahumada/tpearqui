#include <stdint.h>
#include <videoDriver.h>
#include <naiveConsole.h>

void syscall_handler(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t id){
    switch(id){
        case 1:
            puts(rsi, rdx, rcx, r8);
            break;
    }
    return;
}