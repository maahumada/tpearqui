/* sampleCodeModule.c */
#include <syscall.h>
#include <stdint.h>

extern void print(char* str, uint32_t hexacolor, uint64_t x, uint64_t y);

int main() {
	print("FUNCA!", 0xFFAA66, 100, 100);
	return 0;
}