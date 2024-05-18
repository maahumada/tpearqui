#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void putChar(uint8_t c, uint32_t hexacolor, uint64_t x, uint64_t y);

void puts(const char* str, uint32_t hexacolor, uint64_t x, uint64_t y);

#endif
