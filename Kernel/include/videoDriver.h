#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

void putChar(uint8_t c, uint32_t hexacolor);
void removeChar();
void puts(const char* str, uint32_t hexacolor);
void zoomIn();
void zoomOut();

#endif
