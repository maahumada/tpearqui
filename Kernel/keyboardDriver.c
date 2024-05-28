#include <keyboardDriver.h>
#include <videoDriver.h>
#include <interrupts.h>

#define LETTER_WIDTH 8
#define LINE_HEIGHT 12

extern uint8_t readKeyPol();

static char keyMapRow = 0;
#define LEFT_SHIFT  0x2A
#define RIGHT_SHIFT 0x36
#define ALT 0x38
#define ENTER 0x1C
#define BACKSPACE 0x0E

uint64_t x = 0;
uint64_t y = LINE_HEIGHT;

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

static char stdInBuffer[1000];
static uint64_t stdInBufferPosition;

void keyboard_handler(){
  	uint8_t aux = readKeyPol();

	if(aux & 0x80) { // tecla levantada, ignorar
		aux -= 0x80;
		if(aux == LEFT_SHIFT || aux == RIGHT_SHIFT || aux == ALT){
			keyMapRow = 0;
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
		default:
			stdInBuffer[stdInBufferPosition++] = spanish_keyboard_layout[aux][keyMapRow]; 
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
	do{}while(stdInBuffer[stdInBufferPosition - 1] != '\n' && stdInBufferPosition < count);
	for(int i = 0; i < stdInBufferPosition - 1; i++){
	 	buffer[i] = stdInBuffer[i];
	}

	buffer[stdInBufferPosition - 1] = 0;

	_cli();
	return stdInBufferPosition;
}