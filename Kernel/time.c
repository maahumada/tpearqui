#include <time.h>

static unsigned long ticks = 0;
extern uint64_t getHours();
extern uint64_t getMinutes();
extern uint64_t getSeconds();

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

static void num_to_str(uint64_t num, char* buffer){
	if(num > 99)
		return;
	if(num < 10){
		*(buffer) = '0';
		*(buffer+1) = num + '0';
	}
	*(buffer) = num / 10 + '0';
	*(buffer+1) = num % 10 + '0';
}

void get_current_time_string(char* buffer) {
	num_to_str(getHours(), buffer);
	*(buffer+2) = ':';
	num_to_str(getMinutes(), buffer+3);
	*(buffer+5) = ':';
	num_to_str(getSeconds(), buffer+6);
	*(buffer+8) = 0;
}