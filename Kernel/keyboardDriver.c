#include <keyboardDriver.h>
#include <videoDriver.h>
#include <interrupts.h>
#include <registers.h>

#define LETTER_WIDTH 8
#define LINE_HEIGHT 12

extern uint8_t readKeyPol();

static uint64_t keyMapRow = 0;

#define LEFT_SHIFT  0x2A
#define RIGHT_SHIFT 0x36
#define ALT 0x38
#define ENTER 0x1C
#define BACKSPACE 0x0E
#define CAPSLOCK 0x3A
#define CTRL 0x1D

uint64_t x = 0;
uint64_t y = LINE_HEIGHT;

static uint8_t stdInBuffer[1000];
static uint64_t stdInBufferPosition;
static char control = 0;
static char capsLock = 0;

void updateCoords(uint64_t* x, uint64_t* y) {
	*x += LETTER_WIDTH;
	if(*x >= getWidth()) {
		*x = 0;
		*y += LINE_HEIGHT;
	}
}

void numToStr(uint64_t num, char* buffer){
	if(num == 0){
		*(buffer) = '0';
		*(buffer+1) = 0;
		return;
	}
	int digits = 0;
	uint64_t numCpy = num;
	while(numCpy > 0){
		digits++;
		numCpy /= 10;
	}
	for(int i = 1; i <= digits; i++){
		*(buffer+digits-i) = num % 10 + '0';
		num /= 10;
	}
	*(buffer+digits) = 0;
}

void keyboard_handler(){
  uint8_t aux = readKeyPol();

	if(aux & 0x80) { // tecla levantada, ignorar
		aux -= 0x80;
		if(aux == LEFT_SHIFT || aux == RIGHT_SHIFT || aux == ALT){
			keyMapRow = 0;
		}
		if(aux == CTRL) {
			control = 0;
		}
		return;
	}

	switch(aux){
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
			keyMapRow = 1;
			break;
		case ALT:
			keyMapRow = 2;
			break;
		case CTRL:
			control = 1;
			break;
		case CAPSLOCK:
			capsLock = (capsLock == 0) ? 1 : 0;
			break;
		default:
			stdInBuffer[stdInBufferPosition] = spanish_keyboard_layout[aux][keyMapRow]; 
			if(capsLock == 1 && stdInBuffer[stdInBufferPosition] >= 'a' && stdInBuffer[stdInBufferPosition] <= 'z') stdInBuffer[stdInBufferPosition] -= 'a' - 'A';
			if(control == 1 && (stdInBuffer[stdInBufferPosition] == 'R' || stdInBuffer[stdInBufferPosition] == 'r')) {
				updateRegisters();
				stdInBufferPosition--;
			}
			stdInBufferPosition++;
			break;
	}
}

void getChar(uint8_t* character){
	_sti();
	while(stdInBufferPosition == 0){}
	*character = stdInBuffer[0];
	for(int i = 1; i < stdInBufferPosition; i++){
		stdInBuffer[i-1] = stdInBuffer[i];
	}
	stdInBufferPosition--;
	_cli();
}

void getLastPressed(uint8_t* character){
	if(stdInBufferPosition == 0) {
		*character = 0;
	} else {
		*character = stdInBuffer[--stdInBufferPosition];
	}
}

uint64_t read(char* buffer, uint64_t count){
	_sti();
	stdInBufferPosition = 0;
	while(stdInBuffer[stdInBufferPosition - 1] != '\n' && stdInBufferPosition < count){};
	for(int i = 0; i < stdInBufferPosition - 1; i++){
	 	buffer[i] = stdInBuffer[i];
	}

	buffer[stdInBufferPosition - 1] = 0;

	_cli();
	return stdInBufferPosition;
}