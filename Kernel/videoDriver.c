#include <videoDriver.h>
#include <fonts.h>
#include <keyboardDriver.h>

#define LETTER_WIDTH 8
#define LINE_HEIGHT 16

#define LEFT_SHIFT  0x2A
#define RIGHT_SHIFT 0x36
#define ALT 0x38
#define ENTER 0x1C
#define BACKSPACE 0x0E

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

uint8_t reverseBits(uint8_t byte) {
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

void putChar(uint8_t c, uint32_t hexacolor, uint64_t x, uint64_t y) {
    for (int row = 0; row < LETTER_WIDTH; row++) {
        uint8_t row_data = reverseBits(font[(int)c][row]); // Invertir los bits 
        for (int col = 0; col < LETTER_WIDTH; col++) {
            if (row_data & (1 << (LETTER_WIDTH - 1 - col))) {
                putPixel(hexacolor, x + col, y + row);
            }
        }
    }
}

void clearChar(uint64_t x, uint64_t y){
	for (int row = 0; row < LINE_HEIGHT; row++) {
		for(int col = 0; col < LETTER_WIDTH; col++){
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

/*void puts(const char* str, uint32_t hexacolor, uint64_t x, uint64_t y) {
  uint64_t verticalOffset = y;
	while (*str) {	
		if(x >= getWidth()) {
			x = 0;
			verticalOffset += LINE_HEIGHT;
		}else{
			clearChar(x, verticalOffset);
			putChar(*str++, hexacolor, x, verticalOffset);
			x += LETTER_WIDTH; // siguiente caracter 
		}
  }
}*/

void clear(){
	for(int x = 0; x < 1024; x++)
		for(int y = 0; y < 768; y++)
			putPixel(0x000000, x, y);
}

void print(){
	int x = 0;
	int y = 8;
	for(int i = 0; i < BUFFER_SIZE; i++){
		switch(screenBuffer[i].letter) {
			case ENTER:			
				x = 0;
				y += LINE_HEIGHT;	
				break;
			default:
				clearChar(x, y);
				putChar(screenBuffer[i].letter, screenBuffer[i].color, x, y);
				x += LETTER_WIDTH;
				if(x >= getWidth()){
					x = 0;
					y += LINE_HEIGHT;
				}
				break;
		}
	}
}

void puts(char* str, uint32_t color){
	for(int i = 0; str[i] != 0; i++)
		screenBuffer[bufferPosition].letter = str[i];
		screenBuffer[bufferPosition].color = color;
		bufferPosition++;
}