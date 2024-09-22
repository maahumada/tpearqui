#ifndef _STRINGS_H_
#define _STRINGS_H_

#include <stdint.h>

int strcmp(const char* a, const char* b);

int strtoint(const char *str);

char* strcpy(char* dest, const char* src);

void numToStr(uint64_t number, char* str);

void reverseStr(char* str, int length);

void *test_memset(void *ptr, int value, uint64_t num);

#endif