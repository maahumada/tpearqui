#ifndef SPEAKER_DRIVER_H
#define SPEAKER_DRIVER_H

#include <stdint.h>
#include <lib.h>

void play_sound(uint32_t nFrequence);
void nosound();
void beep();

#endif