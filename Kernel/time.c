#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void timer_wait(unsigned long time){
	_sti();
	unsigned long aux = ticks + time;
	while (ticks < aux){
		// busy wait
	}
	_cli();
}