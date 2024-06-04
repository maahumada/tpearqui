#include <stdint.h>
#include <videoDriver.h>
#include <time.h>
#include <keyboardDriver.h>
#include <registers.h>

#define REGISTERS_DIM 17

static uint64_t registers[REGISTERS_DIM];
const char* register_names[REGISTERS_DIM] = {"RIP: ", "RSP: ", "RBP: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RDI: ", "RSI: ", "R8:  ", "R9:  ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};

static void hex_to_ascii(uint64_t hex, char *ascii) {
    // Character array for hex digits
    const char hex_digits[] = "0123456789ABCDEF";
    int i;
    
    // Initialize the ASCII array with null characters
    for (i = 0; i < 17; i++) {
        ascii[i] = '\0';
    }

    // Convert hex to ASCII starting from the least significant digit
    for (i = 15; i >= 0; i--) {
        ascii[i] = hex_digits[hex & 0xF];
        hex >>= 4;
    }
}

static void exceptionDump() {
	for (int i = 0; i < REGISTERS_DIM; i++) {
		puts(register_names[i], 0xeb6d3f);
		puts("0x", 0xFF0000);
		char str[100];
		hex_to_ascii(registers[i], str);
		puts(str, 0xFF0000);
		if(i % 2 == 1) puts("\n", 0x000000);
		else puts("  ", 0x000000);
	}
	puts("\n", 0x000000);
	print();
}

void updateRegisters() { // CTRL + R
    saveAndCopyRegisters(registers);
}

void updateRegistersFromException() { // triggered with exceptions
    copyRegisters(registers);
    exceptionDump(registers);
    puts("\nPress ANY KEY to continue\n", 0xFF0000);
    print();
    uint8_t c;
    getChar(&c);
    clear();
}

void getRegisters(uint64_t* arr) {
    for(int i = 0; i<REGISTERS_DIM; i++) {
        arr[i] = registers[i];
    }
}