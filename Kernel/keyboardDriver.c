#include <keyboardDriver.h>
#include <videoDriver.h>

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

void keyboard_handler(){
  uint8_t aux = readKeyPol();
	uint8_t currentChar;

	if(aux & 0x80) { // tecla levantada, ignorar
		aux -= 0x80;
		if(aux == LEFT_SHIFT || aux == RIGHT_SHIFT || aux == ALT){
			keyMapRow = 0;
		}
		return;
	} 

	char t[2];
	t[1] = 0;
	switch(aux){
		case BACKSPACE:
			remove();
			return;
		case ENTER:
			t[0] = '\n';
			break;
		default:
			t[0] = spanish_keyboard_layout[aux][keyMapRow];
			break;
	}
	puts(t, 0xFFFFFF);
}