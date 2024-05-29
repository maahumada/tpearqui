#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putChar(uint8_t c, uint32_t hexacolor);
void removeChar();
void puts(const char* str, uint32_t hexacolor);
void zoomIn();
void zoomOut();
void putSquare(uint32_t color, uint64_t x, uint64_t y, uint64_t size);
void printImage(uint64_t x, uint64_t y);

#endif