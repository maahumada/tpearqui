#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <naiveConsole.h>
#include <clock.h>

void syscallDispatcher(uint64_t p0, uint64_t p1, uint64_t p2, uint64_t p3, uint64_t p4, uint64_t id){
    puts("START DISPATCH WITH ID: ", 0xFF0000);
    char t[5];
    numToStr(id, t);
    puts(t, 0xFFFFFF);
    puts("\n", 0xFFFFFF);
    print();
    switch(id){
        case 0x01: // Print
            puts(p0, p1);
            break;
        case 0x02: // Read
            puts("STARTING READ\n", 0x0000FF);
            print();
            read(p0, p1);
            puts("FINISH READ\n", 0x0000FF);
            print();
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
            clear();
            break;
        default:
            puts("SYSCALL ??", 0xFF00FF);
            print();
            break;
    }
    puts("FINISH DISPATCH WITH ID: ", 0xFF0000);
    numToStr(id, t);
    puts(t, 0xFFFFFF);
    puts("\n", 0xFFFFFF);
    print();
}