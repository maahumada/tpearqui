#include <syscall.h>
#include <stdint.h>
#include <strings.h>
#include <eliminator.h> 
#include <shell.h>


static char buffer[BUFFER_SIZE];
static uint64_t bufferPosition = 0;

static void getData(){
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
}

int main(){
    uint64_t selection = 0;
	while(1){
    puts("Choose:\n", 0xffffff);
    puts("1. shell\n", 0xffffff);
    puts("2. elimintor\n", 0xffffff);
    printScreen();

    getData();
    selection =strtoint(buffer);

    switch (selection)
    {
    case 1:
        mainShell();
        break;
    
    case 2:
        startEliminator();
        break;
    }
	}
    
}