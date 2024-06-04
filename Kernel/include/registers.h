#ifndef __REGISTERS_H__
#define __REGISTERS_H__

#include <stdint.h>

void updateRegisters();
void updateRegistersFromException();
void getRegisters(uint64_t* arr);

#endif