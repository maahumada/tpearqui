#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <naiveConsole.h>

void syscallDispatcher(uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t id){
    switch(id){
        case 0x01: // Print
            puts(p0, p1);
            break;
        case 0x02: // Read
            read(p0, p1);
            break;
        case 0x04: // PrintScreen
            print();
            break;
        default:
            //puts("SYSCALL ??", 0xFF00FF, 200, 200);
            break;
    }
}