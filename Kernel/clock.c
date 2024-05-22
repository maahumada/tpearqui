#include <stdint.h>

#define TIMEZONE_OFFSET 3
#define HOURS_IN_DAY 24

extern uint64_t getHours();
extern uint64_t getMinutes();
extern uint64_t getSeconds();
extern uint64_t getDay();
extern uint64_t getMonth();
extern uint64_t getYear();

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

uint64_t getHoursUTC3(){
	uint64_t hour = getHours();
	if(hour < TIMEZONE_OFFSET){ 
		return (HOURS_IN_DAY - TIMEZONE_OFFSET) + hour;   
	}
	return hour - TIMEZONE_OFFSET;
}

void get_current_time_string(char* buffer) {
    num_to_str(getDay(), buffer);
    *(buffer+2) = '/';
    num_to_str(getMonth(), buffer+3);
    *(buffer+5) = '/';
    num_to_str(getYear(), buffer+6);
    *(buffer+8) = ' ';
    *(buffer+9) = '-';
    *(buffer+10) = ' ';
    num_to_str(getHoursUTC3(), buffer+11);
    *(buffer+13) = ':';
    num_to_str(getMinutes(), buffer+14);
    *(buffer+16) = ':';
    num_to_str(getSeconds(), buffer+17);
    *(buffer+19) = 0;
}