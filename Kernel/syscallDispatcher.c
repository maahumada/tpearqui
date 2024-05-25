#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <clock.h>

void syscallDispatcher(uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t id){
    switch(id){
        case 0x01: // Print
            puts(p0, p1);
            break;
        case 0x02: // Read
            read(p0, p1);
            break;
        case 0x03: // getChar
            getChar(p0);
            break;
        case 0x04: // PrintScreen
            print();
            break;
        case 0x05:
            get_current_time_string(p0);
            break;
        case 0x06:
            zoomIn();
            break;
        case 0x07:
            zoomOut();
            break;
        case 0x08:
            blackOut();
            break;
        case 0x09: // putChar
            putChar(p0, p1);
            break;
        case 0x0A: // removeChar
            removeChar();
            break;
        case 0x0B:
            getRegisters(p0);
            break;
        case 0x0C:
            clear();
            break;
        default:
            break;
    }
}