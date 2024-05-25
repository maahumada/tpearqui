/* sampleCodeModule.c */
#include <syscall.h>
#include <stdint.h>
#include <strings.h>

#define ENTER '\n'
#define BACKSPACE '\b'

#define COMMANDS_DIM 9
#define REGISTERS_DIM 16

char username[40] = {'u','s','u','a','r','i','o',0};

const char* command_names[COMMANDS_DIM-1] = {"clear", "dump", "eliminator", "help", "time", "zoom-in", "zoom-out", "config"};
const char* command_descriptions[COMMANDS_DIM-1] = {"clears screen", "shows registers status", "starts eliminator", "shows commands", "shows time", "increases text size", "decreases text size", "terminal parameters configuration"};
static const char * notfound = "Command not found\n";

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
	"config"
};

const char* register_names[REGISTERS_DIM] = {"RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8:  ", "R9:  ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};

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
	for (int i = 0; i < REGISTERS_DIM; i++) {
		puts(register_names[i], 0xeb6d3f);
		puts("0x", 0xFFFFFF);
		char str[100];
		hex_to_ascii(registers[REGISTERS_DIM-1-i], str);
		puts(str, 0xFFFFFF);
		if(i % 2 == 1) puts("\n", 0x000000);
		else puts("  ", 0x000000);
	}
	printScreen();
}

void eliminator() {
	puts("eliminator\n", 0xFFFFFF);
	printScreen();
}

void help() {
	for (int i = 0; i < COMMANDS_DIM-1; i++) {
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

void callCommand(int i) {
	switch(i) {
		case 0: 
			clear(); 
			break;
		case 1: 
			dump();
			break;
		case 2: 
			eliminator();
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
	}
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