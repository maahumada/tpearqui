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

static uint64_t getHoursUTC3(uint64_t hour){
	if(hour < TIMEZONE_OFFSET){ 
		return (HOURS_IN_DAY - TIMEZONE_OFFSET) + hour;   
	}
	return hour - TIMEZONE_OFFSET;
}

static int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

void get_current_time_string(char* buffer) {
    int hours = getHours();
    int flag = (hours < TIMEZONE_OFFSET) ? 1 : 0;
    int day = getDay();
    int month = getMonth();
    int year = getYear();
    if(flag) {
        if(day == 1) {
            if(month == 1) {
                year--;
                month = 12;
            } else {
                month--;
            }
            day = daysInMonth[month-1];
        } else {
            day--;
        }
    }
    num_to_str(day, buffer);
    *(buffer+2) = '/';
    num_to_str(month, buffer+3);
    *(buffer+5) = '/';
    num_to_str(year, buffer+6);
    *(buffer+8) = ' ';
    *(buffer+9) = '-';
    *(buffer+10) = ' ';
    num_to_str(getHoursUTC3(hours), buffer+11);
    *(buffer+13) = ':';
    num_to_str(getMinutes(), buffer+14);
    *(buffer+16) = ':';
    num_to_str(getSeconds(), buffer+17);
    *(buffer+19) = 0;
}