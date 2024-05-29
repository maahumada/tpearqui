#include <videoDriver.h>
#include <fonts.h>
#include <keyboardDriver.h>
#include <image.h>

#define LETTER_WIDTH 8

static uint64_t scale = 2;

typedef struct {
	char letter;
	uint32_t color;
} bufferItem;

#define BUFFER_SIZE 6144
	
static bufferItem screenBuffer[BUFFER_SIZE];

static int bufferPosition = 0;

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void putCharPixel(uint8_t c, uint32_t hexacolor, uint64_t x, uint64_t y) {
    for (int row = 0; row < LETTER_WIDTH; row++) {
        uint8_t row_data = font[(int)c][row];
        for (int col = 0; col < LETTER_WIDTH; col++) {
          	if (row_data & (1 << col)) {
							for(int i = 0; i < scale; i++){
								for(int j = 0; j < scale; j++){
									putPixel(hexacolor, x + scale * col + i, y + scale * row + j);
								}
							}
          	} else {
							for(int i = 0; i < scale; i++){
								for(int j = 0; j < scale; j++){
									putPixel(0x000000, x + scale * col + i, y + scale * row + j);
								}
							}
						}
        }
    }
}

void clearChar(uint64_t x, uint64_t y){
	for (int row = 0; row < LETTER_WIDTH * scale; row++) {
		for(int col = 0; col < LETTER_WIDTH * scale; col++){
			putPixel(0x000000, x + col, y + row);
		}
  }
}

uint64_t getWidth() {
	return (uint64_t)VBE_mode_info->width;
}

uint64_t getHeight() {
	return (uint64_t)VBE_mode_info->height;
}

void blackOut(){
	for(int x = 0; x < 1024; x++)
		for(int y = 0; y < 768; y++)
			putPixel(0x000000, x, y);
}

void clear(){
	blackOut();
	for(int i = 0; i < bufferPosition; i++)
		screenBuffer[i].letter = 0;
	bufferPosition = 0;
}

void print(){
	int x = 0;
	int y = LETTER_WIDTH * scale;
	int i;
	for(i = 0; i <= bufferPosition; i++){
		switch(screenBuffer[i].letter) {
			case '\n':
				x = 0;
				y += 2 * LETTER_WIDTH * scale;	
				break;
			default:
				if(x + LETTER_WIDTH * scale > getWidth()) {
					x = 0;
					y += 2 * LETTER_WIDTH * scale;
				}
				putCharPixel(screenBuffer[i].letter, screenBuffer[i].color, x, y);
				x += LETTER_WIDTH * scale;
				break;
		}
		if(y > getHeight()){
			clear();
			return;
		}
	}
}

void puts(const char* str, uint32_t hexacolor){
	for(int i = 0; str[i] != 0; i++){
		screenBuffer[bufferPosition].letter = str[i];
		screenBuffer[bufferPosition].color = hexacolor;
		bufferPosition++;
	}
}

void putsAtPos(const char* str, uint32_t hexacolor, uint64_t position){
	for(int i = 0; str[i] != 0; i++){
		screenBuffer[position+i].letter = str[i];
		screenBuffer[position+i].color = hexacolor;
	}
}

void putChar(uint8_t character, uint32_t hexacolor){
	screenBuffer[bufferPosition].letter = character;
	screenBuffer[bufferPosition].color = hexacolor;
	bufferPosition++;
}

void removeChar(){
	bufferPosition--;
	screenBuffer[bufferPosition].letter = 0;
}

void zoomIn(){
	blackOut();
	if(scale < 4) scale++;
	else puts("MAX ZOOM REACHED\n", 0xFF0000);
}

void zoomOut(){
	blackOut();
	if(scale > 1) scale--;
	else puts("MIN ZOOM REACHED\n", 0xFF0000);
}

void putSquare(uint32_t color, uint64_t x, uint64_t y, uint64_t size){
  for(int i = x; i < x + size; i++){
      for(int j = y; j < y + size; j++) {
          putPixel(color, i, j);
      }
  } 
}

<<<<<<< HEAD
void printImage(uint64_t x, uint64_t y){
	uint64_t size = 6;
	for(int i = 0; i < IMAGE_WIDTH; i++){
		for(int j = 0; j < IMAGE_HEIGHT; j++){
			putSquare(image[i][j], i * size + x, j * size + y, size);
		}
	}
=======
void zeroDivisionExceptionHandler(){
	puts("ERROR: ZERO DIVISION", 0xFF0000);
}

void invalidOpCodeExceptionHandler(){
	puts("ERROR: INVALID OP-CODE", 0xFF0000);
>>>>>>> main
}