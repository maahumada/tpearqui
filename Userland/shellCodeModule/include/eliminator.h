#ifndef ELIMINATOR_H
#define ELIMINATOR_H

#include <stdint.h>

#define WIDTH 128
#define HEIGHT 96
#define SIZE 8
#define START_OFFSET 3
#define BUFFER_SIZE 6144
#define ENTER '\n'
#define BACKSPACE '\b'
#define PLAYER_1_COLOR 0x00FF00
#define PLAYER_2_COLOR 0x0000FF
#define START_DELAY 36

void startEliminator();
void eliminator();

#endif