#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>
#include <interrupts.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void get_current_time_string(char* buffer);
void putPixel();
void timer_wait(unsigned long time);

#endif