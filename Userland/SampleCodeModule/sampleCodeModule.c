/* sampleCodeModule.c */
#include <syscall.h>
#include <stdint.h>

extern void print(char* str, uint32_t hexacolor);
extern void readline(char* buffer, uint64_t count);
extern void printScreen();

char buffer[1000];

int main() {
	print("usuario@ArquiOS", 0x00FF00);
	print(":", 0xFFFFFF);
	print("/", 0x0000FF);
	print("$ ", 0xFFFFFF);
	printScreen();
	print("HOLA", 0xFFFFFF);
	readline(buffer, 1000);
	print("HOLA2", 0xFFFFFF);
	print(buffer, 0xFFFFFF);
	printScreen();
	return 0;
}