// ELIMINATOR
#include <eliminator.h>
#include <syscall.h>
#include <strings.h>
	
static char buffer[BUFFER_SIZE];
static uint64_t bufferPosition = 0;

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
		puts(" Plays With:\n", PLAYER_1_COLOR);
		puts(" W: ", 0xFFFFFF);
		puts("up\n", PLAYER_1_COLOR);
		puts(" A: ", 0xFFFFFF);
		puts("left\n", PLAYER_1_COLOR);
		puts(" S: ", 0xFFFFFF);
		puts("down\n", PLAYER_1_COLOR);
		puts(" D: ", 0xFFFFFF);
		puts("right\n", PLAYER_1_COLOR);

		if(players == 2){
			puts("\n ", 0x00);
			puts(name2, PLAYER_2_COLOR);
			puts(" Plays With:\n", PLAYER_2_COLOR);
			puts(" I: ", 0xFFFFFF);
			puts("up\n", PLAYER_2_COLOR);
			puts(" J: ", 0xFFFFFF);
			puts("left\n", PLAYER_2_COLOR);
			puts(" K: ", 0xFFFFFF);
			puts("down\n", PLAYER_2_COLOR);
			puts(" L: ", 0xFFFFFF);
			puts("right\n", PLAYER_2_COLOR);
		}
		printScreen();
		sleep(41); 
}

static void menu(){
    puts("                         - ELIMINATOR -\n", 0xFF0000);
    printScreen();
		sleep(9);
    
		if(settingsFlag) {
				speed = 0;
				players = 0;
				player1Wins = 0;
				player2Wins = 0;

				while(speed < 1 || speed > 10){
					puts(" SPEED (1-10): ", 0xFF0000);
					printScreen();
					getData();
					speed = strtoint(buffer);
				}

				while(players < 1 || players > 2){
					puts(" PLAYERS (1-2): ", 0xFF0000);
					printScreen();
					getData();
					players = strtoint(buffer);
				}


				puts(" PLAYER 1's NAME: ", 0xFF0000);
				printScreen();
				getData();
				strcpy(name1, buffer);

				if(players == 2){
					puts(" PLAYER 2's NAME: ", 0xFF0000);
					printScreen();
					getData();
					strcpy(name2, buffer);
				}
		}

    printScreen();

    sleep(18);    

	instructions();
}

static void clearInput(){
		uint8_t c = 1;
		while(c){
				getLastPressed(&c);
		}
}

static void nextGame() {
	sleep(50);
	clearScreen();
	clearInput();

	if(players == 2) {
		puts(" SCORE: \n", 0xFFFFFF);
		puts("  - ", 0xFFFFFF);
		puts(name1, 0x00FF00);
		puts(": ", 0x00FF00);
		char wins1str[20] = { 0 };
		numToStr(player1Wins, wins1str);
		puts(wins1str, 0xFF0000);
		puts("\n", 0x000000);

		puts("  - ", 0xFFFFFF);
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

		puts("\n Press ", 0xFFFFFF);
		puts("[ENTER]", 0x00FF00);
		puts(" to play again\n", 0xFFFFFF);

		puts(" Press ", 0xFFFFFF);
		puts("ANY OTHER KEY", 0x00FF00);
		puts(" to go back to the main menu of the game\n", 0xFFFFFF);
		printScreen();
		uint8_t c;
		getChar(&c);
		if(c == ENTER) {
			eliminator();
			return;
		} 
	} 
	clearScreen();
	puts("\n Press ", 0xFFFFFF);
	puts("[E]", 0x00FF00);
	puts(" to exit game\n", 0xFFFFFF);

	puts(" Press ", 0xFFFFFF);
	puts("[S]", 0x00FF00);
	puts(" to save default settings and play again\n", 0xFFFFFF);


	puts(" Press ", 0xFFFFFF);
	puts("ANY OTHER KEY", 0x00FF00);
	puts(" to reset settings and play again\n", 0xFFFFFF);

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
		puts(" ", 0x000000);
		puts(name2, 0x0000FF);
		puts(" wins\n", 0xFF0000);
	}
    printScreen();
	nextGame();
}

static void lostPlayer2(){
	if(players == 2) player1Wins++;
    blackOut();
	puts(" ", 0x000000);
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
		score = 0;
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
	eliminator();
}