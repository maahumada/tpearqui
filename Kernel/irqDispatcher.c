#include <time.h>
#include <stdint.h>
#include <keyboardDriver.h>
#include <naiveConsole.h>

static void int_20();
static void int_21();
static void int_80(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t id);

void irqDispatcher(uint64_t irq /* rdi */, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t id) { // tengo que poner los demás registros asi le llegan al syscall
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
		case 0x80:
			int_80(rsi, rdx, rcx, r8, id);
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21() {
	keyboard_handler();
}

static void int_80(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t id){
	// ncNewline();
	// ncPrint(rsi);
	// ncNewline();
	// ncNewline();
	// ncPrintHex(rdx);
	// ncNewline();
	// ncNewline();
	// ncPrintHex(rcx);
	// ncNewline();
	// ncNewline();
	// ncPrintHex(r8);
	// ncNewline();
	// ncNewline();
	// ncPrintHex(id);
	// ncNewline();


	syscall_handler(rsi, rdx, rcx, r8, id); // en r9 tengo a RAX que es donde llega el id del syscall
}