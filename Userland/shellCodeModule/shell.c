/* sampleCodeModule.c */
#include <syscall.h>
#include <stdint.h>
#include <strings.h>
#include <eliminator.h> 

#define ENTER '\n'
#define BACKSPACE '\b'

#define COMMANDS_DIM 11
#define REGISTERS_DIM 17

char username[40] = {'u','s','u','a','r','i','o',0};

const char* command_names[COMMANDS_DIM-1] = {"clear", "dump", "eliminator", "help", "time", "zoom-in", "zoom-out", "config", "exception00", "exception06"};
const char* command_descriptions[COMMANDS_DIM-1] = {"clears screen", "shows registers status", "starts eliminator", "shows commands", "shows time", "increases text size", "decreases text size", "terminal parameters configuration", "triggers exception 0x00", "triggers exception 0x06"};
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
	"config",
	"exception00",
	"exception06"
};

const char* register_names[REGISTERS_DIM] = {"RSP: ", "RIP: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "R8:  ", "R9:  ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: "};

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
	puts("\n", 0x000000);
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





// ELIMINATOR
	
// static char buffer[BUFFER_SIZE];
// static uint64_t bufferPosition = 0;

#define WIDTH 128
#define HEIGHT 96
#define SIZE 8
#define START_OFFSET 3
#define PLAYER_1_COLOR 0x00FF00
#define PLAYER_2_COLOR 0x0000FF
#define START_DELAY 36

static char occupied[HEIGHT][WIDTH];

static uint64_t speed = 0;
static char players = 0;
static char name1[40];
static char name2[40];
static char settingsFlag = 1;

static uint64_t score;
static uint64_t player1Wins = 0, player2Wins = 0;

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

static void instructions(){
	puts("\n ", 0x00);
	puts(name1, PLAYER_1_COLOR);
	puts(" plays with:\n A: left\n W: up\n D: right\n S:down\n", PLAYER_1_COLOR);
	if(players == 2){
		puts("\n", 0x00);
		puts(name2, PLAYER_2_COLOR);
		puts(" plays with:\n J: left\n I: up\n L: right\n K:down\n", PLAYER_2_COLOR);
	}
	printScreen();
	sleep(41); 
}

static void menu(){
    puts("ELIMINATOR\n", 0xFF0000);
	puts("----------\n\n", 0xFF0000);
    printScreen();
	sleep(9);
    
	if(settingsFlag) {
		speed = 0;
		players = 0;
		while(speed < 1 || speed > 10){
			puts("SPEED (1-10): ", 0xFF0000);
			printScreen();
			getData();
			speed = strtoint(buffer);
		}

		while(players < 1 || players > 2){
			puts("PLAYERS (1-2): ", 0xFF0000);
			printScreen();
			getData();
			players = strtoint(buffer);
		}

		
		puts("PLAYER 1's NAME: ", 0xFF0000);
		printScreen();
		getData();
		strcpy(name1, buffer);
		
		if(players == 2){
			puts("PLAYER 2's NAME: ", 0xFF0000);
			printScreen();
			getData();
			strcpy(name2, buffer);
		}
	}

    printScreen();

    sleep(18);    

	instructions();
}

static void nextGame() {
	sleep(50);
	clear();

	if(players == 2) {
		puts(name1, 0x00FF00);
		puts(": ", 0x00FF00);
		char wins1str[20] = { 0 };
		numToStr(player1Wins, wins1str);
		puts(wins1str, 0xFF0000);
		puts("\n", 0x000000);
		
		puts(name2, 0x0000FF);
		puts(": ", 0x0000FF);
		char wins2str[20] = { 0 };
		numToStr(player2Wins, wins2str);
		puts(wins2str, 0xFF0000);
		puts("\n", 0x000000);
		printScreen();
		sleep(30);
	}
	if(settingsFlag == 0) {
		puts("Press [ENTER] to play again or any other key to change settings or quit\n", 0xFF0000);
		printScreen();
		uint8_t c;
		getChar(&c);
		if(c == ENTER) {
			eliminator();
			return;
		} 
	} 
	clear();
	puts("Press [E] to exit game\n", 0xFF0000);
	puts("Press [S] to save default settings or any other key to play\n", 0xFF0000);
	printScreen();
	uint8_t c2;
	getChar(&c2);
	if(c2 == 'e' || c2 == 'E') {
		blackOut();
		clearScreen();
		return;
	}
	if(c2 == 's' || c2 == 'S') {
		settingsFlag = 0;
	} else {
		player1Wins = 0;
		player2Wins = 0;
		settingsFlag = 1;
	}

	eliminator();
}

static void tie(){
    blackOut();
    puts("It's a tie!", 0xFF0000);
    printScreen();
	nextGame();
}

static void lostPlayer1(){
	if(players == 2) player2Wins++;
    blackOut();
	if(players == 1){
		puts("You lost! Score: ", 0xFF0000);
		char scoreStr[20] = { 0 };
		numToStr(score * 3, scoreStr);
		puts(scoreStr, 0xFF0000);
		puts("\n", 0xFF0000);
	} else {
		puts(name2, 0x0000FF);
		puts(" wins\n", 0xFF0000);
	}
    printScreen();
	nextGame();
}

static void lostPlayer2(){
	if(players == 2) player1Wins++;
    blackOut();
	puts(name1, 0xFF0000);
    puts(" wins\n", 0xFF0000);
    printScreen();
	nextGame();
}

static void setBorder(){
  for(int i = 0; i < WIDTH; i++){
    occupied[0][i] = 1;
    occupied[HEIGHT - 1][i] = 1;
	putSquare(0xFF0000, i * SIZE, 0, SIZE);
	putSquare(0xFF0000, i * SIZE, (HEIGHT - 1) * SIZE, SIZE);
  }
  for(int i = 1; i < HEIGHT - 1; i++){
    occupied[i][0] = 1;
    occupied[i][WIDTH - 1] = 1;
	putSquare(0xFF0000, 0, i * SIZE, SIZE);
	putSquare(0xFF0000, (WIDTH - 1) * SIZE, i * SIZE, SIZE);
  }
}

void clearOccupied(){
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			occupied[i][j] = 0;
		}
	}
}

void eliminator() {
	clearOccupied();
    clearScreen();
    menu();
    clearScreen();
    setBorder();
    //arranca el juego
    uint8_t lastKeyPlayer1 = 's', lastKeyPlayer2 = 'i';
    int x1 = WIDTH / 2, y1 = START_OFFSET, x2 = WIDTH / 2 + 1, y2 = HEIGHT - START_OFFSET;
    uint8_t finished = 0;
    while(1){
        // Check Tie
        if(occupied[y1][x1] != 0 && occupied[y2][x2] != 0) { 
			makeBeep();
            tie();
            finished = 1;
        }
        // Check Player 1 loses
        if(occupied[y1][x1] != 0) { 
			makeBeep();
            lostPlayer1();
            finished = 1;
        }
        // Check Player 2 los
        if(occupied[y2][x2] != 0) {
			makeBeep();
            lostPlayer2();
            finished = 1;
        }

		if(finished)
			break;

		
		// Update player 1 position
        occupied[y1][x1] = 1;
        putSquare(PLAYER_1_COLOR, x1 * SIZE, y1 * SIZE, SIZE);
        
		// Update player 2 position
		if(players == 2){
			occupied[y2][x2] = 1;
			putSquare(PLAYER_2_COLOR, x2 * SIZE, y2 * SIZE, SIZE);
		} 
		
        //Get keyboard inputs
        uint8_t c = 1;
        while(c) {
            getLastPressed(&c);
            if(c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
            switch(c) {
                case 'w': 
					lastKeyPlayer1 = (lastKeyPlayer1 == 's') ? lastKeyPlayer1 : c;
					break;
                case 'a': 
					lastKeyPlayer1 = (lastKeyPlayer1 == 'd') ? lastKeyPlayer1 : c;
					break;
				case 's':
					lastKeyPlayer1 = (lastKeyPlayer1 == 'w') ? lastKeyPlayer1 : c;
					break;
				case 'd':
					lastKeyPlayer1 = (lastKeyPlayer1 == 'a') ? lastKeyPlayer1 : c;
					break;
                case 'i': 
					lastKeyPlayer2 = (lastKeyPlayer2 == 'k') ? lastKeyPlayer2 : c;
					break;
                case 'j': 
					lastKeyPlayer2 = (lastKeyPlayer2 == 'l') ? lastKeyPlayer2 : c;
					break;
				case 'k':
					lastKeyPlayer2 = (lastKeyPlayer2 == 'i') ? lastKeyPlayer2 : c;
					break; 
                case 'l':
					lastKeyPlayer2 = (lastKeyPlayer2 == 'j') ? lastKeyPlayer2 : c;
					break; 
            }
        }

        // Move player 1 
        switch(lastKeyPlayer1) {
            case 'w': 
                y1--;
                break;
            case 'a':
                x1--;
                break;
            case 's':
                y1++;
                break;
            case 'd':
                x1++;
                break;
        }
  
        
        if(players == 2) {
            // Update direction player 2
            switch(lastKeyPlayer2) {
                case 'i':
                    y2--;
                    break;
                case 'j':
                    x2--;
                    break;
                case 'k':
                    y2++;
                    break;
                case 'l':
                    x2++;
                    break;
            }
        }

		score++;
			
        sleep(11 - speed);
    }     
}

void startEliminator() {
	settingsFlag = 1;
	score = 0;
	eliminator();
}
