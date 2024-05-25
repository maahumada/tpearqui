#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <stdint.h>

extern void print(char * str, uint32_t hexacolor);
extern void read(char * buffer, uint64_t count);
extern void printScreen();
extern void getChar();
extern void getTimeString(char* buffer);
extern void zoomInScreen();
extern void zoomOutScreen();
extern void clearScreen();


#endif