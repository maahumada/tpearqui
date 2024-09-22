#ifndef _SYSCALL_H_
#define _SYSCALL_H_

#include <stdint.h>

extern void puts(char * str, uint32_t hexacolor);
extern void read(char * buffer, uint64_t count);
extern void printScreen();
extern void getChar();
extern void putChar();
extern void removeChar();
extern void getTimeString(char* buffer);
extern void zoomInScreen();
extern void zoomOutScreen();
extern void blackOut();
extern void getRegisters(uint64_t* registers);
extern void clearScreen();
extern void putSquare(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t size);
extern void sleep(uint64_t time);
extern void getLastPressed(uint8_t* character);
extern void makeBeep(uint32_t nFrequence, unsigned long time);
extern void printImage(uint64_t x, uint64_t y);
extern void getWidth(uint64_t* num);
extern void getHeight(uint64_t* num);
extern void* malloc(uint64_t size);
extern void free(void* p);

#endif