#include <stdint.h>

#define REGISTERS_DIM 17

static uint64_t registers[REGISTERS_DIM];

void updateRegisters() {
    copyRegisters(registers, 0x50);
}

void getRegisters(uint64_t* arr) {
    for(int i = 0; i<REGISTERS_DIM; i++) {
        *(arr+i) = registers[i];
    }
}