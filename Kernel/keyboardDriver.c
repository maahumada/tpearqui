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

	// PRINTEO DE X E Y DEBUG
	char xString[10] = { 0 };
	char yString[10] = { 0 };
	puts("-------", 0xFFFFFF, 0, 300);
	puts("-------", 0xFFFFFF, 0, 320);
	numToStr(x, xString);
	numToStr(y, yString);
	puts("x: ", 0x0000FF, 0, 300);
	puts(xString, 0x0000FF, 24, 300);
	puts("y: ", 0x00FF00, 0, 320);
	puts(yString, 0x00FF00, 24, 320);
	
	switch(aux) {
		case LEFT_SHIFT:
		case RIGHT_SHIFT:
			keyMapRow = 1;
			break;
		case ALT:
			keyMapRow = 2;
			break;
		case ENTER:			
			x = 0;
			y += LINE_HEIGHT;	
			break;
		case BACKSPACE:
			if((int64_t)x - LETTER_WIDTH < 0){
				x = LETTER_WIDTH * (getWidth() / LETTER_WIDTH) - LETTER_WIDTH;
				y -= LINE_HEIGHT;
			}else{
				x -= LETTER_WIDTH;
			}
			clearChar(x, y);
			putChar('X', 0xFF0000, x, y);
			break;
		default:
			currentChar = spanish_keyboard_layout[aux][keyMapRow];
			clearChar(x, y);
			putChar(currentChar, 0xFFFFFF, x, y);
			updateCoords(&x, &y);
			break;
		}

		puts("-------", 0xFFFFFF, 0, 340);
		puts("-------", 0xFFFFFF, 0, 360);
		numToStr(x, xString);
		numToStr(y, yString);
		puts("x: ", 0x0000FF, 0, 340);
		puts(xString, 0x0000FF, 24, 340);
		puts("y: ", 0x00FF00, 0, 360);
		puts(yString, 0x00FF00, 24, 360);
}