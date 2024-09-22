#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <speakerDriver.h>
#include <clock.h>
#include <interrupts.h>
#include <registers.h>
#include <time.h>
#include <mm.h>

static void * const freeMemoryStartAddress = (void*)0x600000;

void syscallDispatcher(uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t id){
    switch(id){
        case 0x01: // Print
            puts((char*)p0, p1);
            break;
        case 0x02: // Read
            read((char*)p0, p1);
            break;
        case 0x03: // getChar
            getChar((uint8_t*)p0);
            break;
        case 0x04: // PrintScreen
            print();
            break;
        case 0x05:
            get_current_time_string((char*)p0);
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
            getRegisters((uint64_t*)p0);
            break;
        case 0x0C:
            clear();
            break;
        case 0x0D:
            putSquare(p0,p1,p2,p3);
            break;
        case 0x0E:
            timer_wait(p0);
            break;
        case 0x0F:
            getLastPressed((uint8_t*)p0);
            break;
        case 0x10:
            beep(p0, p1);
            break;
        case 0x11:
            printImage(p0, p1);
            break;
        case 0x12:
            getWidthInPointer((uint64_t*)p0);
            break;
        case 0x13:
            getHeightInPointer((uint64_t*)p0);
            break;
        case 0x14:
            mm_malloc(p0);
            break;
        case 0x15:
            mm_free((void*)p0);
            break;
        default:
            break;
    }
}