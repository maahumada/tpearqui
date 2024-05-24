/* sampleCodeModule.c */
#include <syscall.h>
#include <stdint.h>
#include <strings.h>

#define COMMANDS_DIM 7

extern void print(char* str, uint32_t hexacolor);
extern void readline(char* buffer, uint64_t count);
extern void getChar();
extern void printScreen();
extern void getTimeString(char* buffer);
extern void zoomInScreen();
extern void zoomOutScreen();
extern void clearAux();


static const char * notfound = "Command not found\n";
static const char * helpDump = "clear: clears screen.\ndump: shows registers status.\neliminator: starts game\ntime: shows time\n";


char buffer[1000];
static char *commands[COMMANDS_DIM] = {
	"clear",
	"dump",
	"eliminator",
	"help",
	"time",
	"zoom-in",
	"zoom-out"
};

void printPrompt(){
	print("usuario@ArquiOS", 0x00FF00);
	print(":", 0xFFFFFF);
	print("/", 0x0000FF);
	print("$ ", 0xFFFFFF);
	printScreen();
}

void clear(){
	print("clear\n", 0x00FF00);
	printScreen();
}


void dump() {
	print("Dump:\n", 0xFFFFFF);
	printScreen();
}

void eliminator() {
	print("eliminator\n", 0xFFFFFF);
	printScreen();
}

void help() {
	print(helpDump, 0xFFFFFF);
	printScreen();
}

void time() {
	char timeBuffer[20] = { 0 };
	getTimeString(timeBuffer);
	print(timeBuffer, 0xFF77FF);
	print("\n", 0xFF77FF);
	printScreen();
}

void zoomIn(){
	print("zoom in\n", 0x223376);
	printScreen();
	// clearAux();
	// zoomInScreen();
}

void zoomOut(){
	print("zoom out\n", 0x981472);
	printScreen();
	// clearAux();
	// zoomOutScreen();
} 


void notFound(){
	print(notfound, 0xFF0000);
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
	}
}

int main() {
	while(1){
		printPrompt();
		print("\nREADING...\n", 0xFF0000);
		printScreen();
		readline(buffer, 1000);
		print("\nREAD DONE\n", 0xFF0000);
		printScreen();
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