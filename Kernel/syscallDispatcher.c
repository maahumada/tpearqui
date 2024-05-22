#include <stdint.h>
#include <videoDriver.h>
#include <naiveConsole.h>

void syscallDispatcher(uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t id){
    ncPrint("syscall dispatcher");
    switch(id){
        case 0x01:
            puts(p0, p1, p2, p3);
            break;
        default:
            puts("SYSCALL ??", 0xFF00FF, 200, 200);
            break;
    }
    return;
}