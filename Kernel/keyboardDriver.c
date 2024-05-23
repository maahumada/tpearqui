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

static char stdInBuffer[1000];
static uint64_t stdInBufferPosition;

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

	//PRINT TO STDOUT
	// char t[2];
	// t[1] = 0;
	// char c;
	// switch(aux){
	// 	case BACKSPACE:
	// 		remove();
	// 		return;
	// 	case ENTER:
	// 		c = '\n';
	// 		break;
	// 	default:
	// 		c = spanish_keyboard_layout[aux][keyMapRow];
	// 		break;
	// }
	// t[0] = c;
	// puts(t, 0xFFFFFF);
}

uint64_t read(char* buffer, uint64_t count){
	stdInBufferPosition = 0;
	while(stdInBufferPosition == 0 || (stdInBuffer[stdInBufferPosition - 1] != '\n' && stdInBufferPosition < count)){
		keyboard_handler();
	}
	for(int i = 0; i < stdInBufferPosition; i++){
	 	buffer[i] = stdInBuffer[i];
	}
	puts("OUT OF FOR\n", 0xFF0000);
	print();
	buffer[stdInBufferPosition] = 0;
	puts("RETURNING READ\n", 0xFF0000);
	print();
	return stdInBufferPosition;
};