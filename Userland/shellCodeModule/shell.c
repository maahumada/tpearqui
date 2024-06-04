/* sampleCodeModule.c */
#include <syscall.h>
#include <stdint.h>
#include <strings.h>
#include <eliminator.h> 
#include <exceptions.h>

#define ENTER '\n'
#define BACKSPACE '\b'

#define COMMANDS_DIM 15
#define HIDDEN_COMMANDS_DIM 2
#define REGISTERS_DIM 17

#define IMAGE_X 0
#define IMAGE_Y 0

char username[40] = {'u','s','u','a','r','i','o',0};

char* command_names[COMMANDS_DIM-HIDDEN_COMMANDS_DIM] = {"clear", "dump", "eliminator", "help", "time", "zoom-in", "zoom-out", "config", "exception00", "exception06", "image", "piano", "mandelbrot"};
char* command_descriptions[COMMANDS_DIM-HIDDEN_COMMANDS_DIM] = {"clears screen", "shows registers status", "starts eliminator", "shows commands", "shows time", "increases text size", "decreases text size", "terminal parameters configuration", "triggers exception 0x00", "triggers exception 0x06", "inspirational art", "playeble piano", "mandelbrot calculation"};
static char * notfound = "Command not found\n";

#define BUFFER_SIZE 6144
	
static char buffer[BUFFER_SIZE];
static uint64_t bufferPosition = 0;

static char *commands[COMMANDS_DIM] = {
	"clear",
	"dump",
	"eliminator",
	"help",
	"time",
	"zoom-in",
	"zoom-out",
	"",
	"config",
	"exception00",
	"exception06",
	"image",
	"ls",
	"piano",
	"mandelbrot"
};

char* register_names[REGISTERS_DIM] = {"RIP: ", "RSP: ", "RBP: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RDI: ", "RSI: ", "R8:  ", "R9:  ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};
 
void printPrompt(){
	puts(username, 0x00FF00);
	puts("@ArquiOS", 0x00FF00);
	puts(":", 0xFFFFFF);
	puts("/", 0x0000FF);
	puts("$ ", 0xFFFFFF);
	printScreen();
}

void clear(){
	clearScreen();
}

void hex_to_ascii(uint64_t hex, char *ascii) {
    // Character array for hex digits
    const char hex_digits[] = "0123456789ABCDEF";
    int i;
    
    // Initialize the ASCII array with null characters
    for (i = 0; i < 17; i++) {
        ascii[i] = '\0';
    }

    // Convert hex to ASCII starting from the least significant digit
    for (i = 15; i >= 0; i--) {
        ascii[i] = hex_digits[hex & 0xF];
        hex >>= 4;
    }
}

void dump() {
	uint64_t registers[REGISTERS_DIM];
	getRegisters(registers);
	int flag = 0;
	for(int i = 0; i< REGISTERS_DIM; i++) {
		if (registers[i] != 0) flag = 1;
	}
	if(flag == 0) {
		puts("No saved registers. Press CTRL + R to save them when desired.\n", 0xeb6d3f);
	} else {
		for (int i = 0; i < REGISTERS_DIM; i++) {
			puts(register_names[i], 0xeb6d3f);
			puts("0x", 0xFFFFFF);
			char str[100];
			hex_to_ascii(registers[i], str);
			puts(str, 0xFFFFFF);
			if(i % 2 == 1) puts("\n", 0x000000);
			else puts("  ", 0x000000);
		}
		puts("\n", 0x000000);
	}
	printScreen();
}

void help() {
	for (int i = 0; i < COMMANDS_DIM-HIDDEN_COMMANDS_DIM; i++) {
		puts(" - ", 0xFFFFFF);
		puts(command_names[i], 0xeb6d3f);
		puts(": ", 0xFFFFFF);
		puts(command_descriptions[i], 0xFFFFFF);
		puts("\n", 0x000000);
	}
	printScreen();
}

void time() {
	char timeBuffer[20] = { 0 };
	getTimeString(timeBuffer);
	puts(" ", 0x000000);
	puts(timeBuffer, 0xFF77FF);
	puts("\n", 0xFF77FF);
	printScreen();
}

void zoomIn(){
	zoomInScreen();
}

void zoomOut(){
	zoomOutScreen();
} 

void notFound(){
	puts(notfound, 0xFF0000);
	printScreen();
}

void noCommand(){
	//puts("\n", 0x000000);
	putChar('\n', 0xFFFFFF);
	printScreen();
}

void config(){
	puts("Nombre de usuario: ", 0xFFFFFF);
	int index = 0;
	uint8_t current;
	while(current != ENTER) {
 		if(current == BACKSPACE) {
			if(index > 0){
				username[--index] = 0;
				removeChar();
			}
		} else if(current != 0) {
			username[index++] = current;
			putChar(current, 0xFFFFFF);
		}
		printScreen();
		getChar(&current);
	}
	username[index] = 0;
	putChar('\n', 0xFFFFFF);
	printScreen();
}

void exception00Tester(){
	exception00();
}

void exception06Tester(){
	exception06();
}

void image(){
	printImage(IMAGE_X, IMAGE_Y);
	uint8_t c;
	getChar(&c);
	clearScreen();
}

void list(){
	puts(". .. ", 0x0000FF);
	puts("ELIMINATOR\n", 0x00FF00);
	printScreen();
}

// Define the frequencies for the notes (A4 = 440Hz)
#define C4  261
#define D4  294
#define E4  329
#define F4  349
#define G4  392
#define A4  440
#define B4  493
#define C5  523

// Map keys to frequencies
int getFrequency(char key) {
    switch (key) {
        case 'a': return C4;  // C4
        case 's': return D4;  // D4
        case 'd': return E4;  // E4
        case 'f': return F4;  // F4
        case 'g': return G4;  // G4
        case 'h': return A4;  // A4
        case 'j': return B4;  // B4
        case 'k': return C5;  // C5
        default: return 0;    // No sound
    }
}

void printPiano(char key, uint32_t pressedColor, uint32_t unpressedColor) {
		clear();

		puts(" - PIANO -\n", 0xFFFFFF);
		puts(" Press [E] to exit\n", 0xFFFFFF);
		puts(" A: ", 0x00FFFF);
		puts("DO / ", (key == 'a') ? pressedColor : unpressedColor);
		
		puts("S: ", 0x00FFFF);
		puts("RE /", (key == 's') ? pressedColor : unpressedColor);

		puts("S: ", 0x00FFFF);
		puts("MI /", (key == 'd') ? pressedColor : unpressedColor);

		puts("F: ", 0x00FFFF);
		puts("FA /", (key == 'f') ? pressedColor : unpressedColor);
		
		puts("G: ", 0x00FFFF);
		puts("SOL /", (key == 'g') ? pressedColor : unpressedColor);

		puts("H: ", 0x00FFFF);
		puts("LA /", (key == 'h') ? pressedColor : unpressedColor);

		puts("J: ", 0x00FFFF);
		puts("SI /", (key == 'j') ? pressedColor : unpressedColor);

		puts("K: ", 0x00FFFF);
		puts("DO", (key == 'k') ? pressedColor : unpressedColor);

		printScreen();
}

void piano() {
    char key;
	uint32_t pressedColor = 0xFFFFFF;
	uint32_t unpressedColor = 0xFF0000;

	printPiano(0, unpressedColor, unpressedColor);

    while (1) {
        getChar(&key); // Get the pressed key
		if(key >= 'A' && key <= 'Z') key -= 'A' - 'a';
		
		if(key == 'e') {
			clearScreen();
			return;
		}
		
        uint32_t frequency = getFrequency(key); // Get the frequency for the key
        if (frequency > 0) {
            makeBeep(frequency, 6); // Play the note for 500 ticks (adjust duration as needed)
        }

		printPiano(key, pressedColor, unpressedColor);
    }
  }

void mandelbrot(){
	uint64_t xres, yres;
	getWidth(&xres);
	getHeight(&yres);
	double xmax = 0.6, ymax = 1.2;
	double xmin = -1.5, ymin = -1.2;
	double dx=(xmax-xmin)/xres;
  double dy=(ymax-ymin)/yres;
	int maxiter = 100;

  double x, y; /* Coordinates of the current point in the complex plane. */
  double u, v; /* Coordinates of the iterated point. */
  int i,j; /* Pixel counters */
  int k; /* Iteration counter */
  for (j = 0; j < yres; j++) {
    y = ymax - j * dy;
    for(i = 0; i < xres; i++) {
      double u = 0.0;
      double v= 0.0;
      double u2 = u * u;
      double v2 = v*v;
      x = xmin + i * dx;
      /* iterate the point */
      for (k = 1; k < maxiter && (u2 + v2 < 4.0); k++) {
            v = 2 * u * v + y;
            u = u2 - v2 + x;
            u2 = u * u;
            v2 = v * v;
      };
      /* compute  pixel color and write it to file */
      if (k >= maxiter) {
        /* interior */
				putSquare(0x000000, i, j, 1);
      }
      else {
        /* exterior */
        uint32_t color = (k >> 16) | (k >> 8) | k; // Grayscale color based on iteration count
        putSquare(color * 12, i, j, 1);
      };
    }
  }
	uint8_t c;
	getChar(&c);
	clearScreen();
}

void callCommand(int i) {
	switch(i) {
		case 0: 
			clear(); 
			break;
		case 1: 
			dump();
			break;
		case 2: 
			startEliminator();
			break;
		case 3: 
			help();
			break;
		case 4: 
			time();
			break;
		case 5:
			zoomIn();
			break;
		case 6: 
			zoomOut();
			break;
		case 7:
			noCommand();
			break;
		case 8:
			config();
			break;
		case 9:
			exception00Tester();
			break;
		case 10:
			exception06Tester();
			break;
		case 11:
			image();
			break;
		case 12:
			list();
			break;
		case 13:
			piano();
      break;
    case 14:
			mandelbrot();
			break;
	}
}


int main() {
	while(1){
		printPrompt();
		// Get keyboard input
		bufferPosition = 0;
		uint8_t current;
		getChar(&current);
		while(current != ENTER) {
 	 		if(current == BACKSPACE) {
				if(bufferPosition > 0){
					buffer[--bufferPosition] = 0;
					removeChar();
				}
			} else if(current != 0) {
				buffer[bufferPosition++] = current;
				putChar(current, 0xFFFFFF);
			}
			printScreen();
			getChar(&current);
		}
		putChar('\n', 0xFFFFFF);
		printScreen();
		buffer[bufferPosition] = 0;
		
    // Select and execute command
		int found = 0;
		for(int i = 0; i < COMMANDS_DIM && !found; i++){
			if(strcmp(buffer, commands[i]) == 0){
				callCommand(i);
				found = 1;
			}
		}
		if(!found){
			notFound();
		}	
	}
	return 0;
}