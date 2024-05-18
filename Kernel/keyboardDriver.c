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

uint16_t x = 0;
uint16_t y = LETTER_WIDTH;

void updateCoords(uint16_t* x, uint16_t* y) {
	if(*x >= getWidth()) {
		*x = 0;
		*y += LINE_HEIGHT;
	} else {
		*x += LETTER_WIDTH;
	}
}

void keyboard_handler(){
  	uint8_t aux = readKeyPol();
	uint64_t verticalOffset = 10;
	uint8_t currentChar;

	if(aux & 0x80) { // tecla levantada, ignorar
		aux -= 0x80;
		if(aux == LEFT_SHIFT || aux == RIGHT_SHIFT || aux == ALT){
			keyMapRow = 0;
		}
		return;
	} 
	
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
			x -= LETTER_WIDTH;
			if(x < 0) {
				x = LETTER_WIDTH * (getWidth() / LETTER_WIDTH);
				y -= LINE_HEIGHT;
			}
			clearChar(x, y);
			break;
		default:
			currentChar = spanish_keyboard_layout[aux][keyMapRow];
			clearChar(x, y);
			putChar(currentChar, 0xFFFFFF, x, y);
			updateCoords(&x, &y);
			break;
		}
}