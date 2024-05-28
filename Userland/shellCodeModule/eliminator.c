// #include <syscall.h>
// #include <stdint.h>
// #include <strings.h>
// #include <eliminator.h>
	
// // static char buffer[BUFFER_SIZE];
// // static uint64_t bufferPosition = 0;

// static char occupied[HEIGHT][WIDTH];

// static uint64_t speed = 1;
// static char players = 1;
// static char * name1 = "YOU";
// static char * name2 = "ELIMINATOR";

// static void getData(){
//     bufferPosition = 0;
// 		uint8_t current;
// 		getChar(&current);
// 		while(current != ENTER) {
//  	 		if(current == BACKSPACE) {
// 				if(bufferPosition > 0){
// 					buffer[--bufferPosition] = 0;
// 					removeChar();
// 				}
// 			} else if(current != 0) {
// 				buffer[bufferPosition++] = current;
// 				putChar(current, 0xFFFFFF);
// 			}
// 			printScreen();
// 			getChar(&current);
// 		}
// 		putChar('\n', 0xFFFFFF);
// 		printScreen();
// 		buffer[bufferPosition] = 0;
// }

// static void menu(){
//     puts("ELIMINATOR\n", 0xFF0000);
//     printScreen();
    
//     while(speed < 1 || speed > 10){
//         puts("SPEED (1-10): ", 0xFF0000);
//         printScreen();
//         getData();
//         speed = strtoint(buffer);
//     }

//     while(players < 1 || players > 2){
//         puts("PLAYERS (1-2): ", 0xFF0000);
//         printScreen();
//         getData();
//         players = strtoint(buffer);
//     }

    
//     puts("PLAYER 1's NAME: ", 0xFF0000);
//         printScreen();
//         getData();
//         name1 = strcpy(name1,buffer);
    
//     if(players == 2){
//         puts("PLAYER 2's NAME: ", 0xFF0000);
//         printScreen();
//         getData();
//         name2 = strcpy(name2,buffer);
//     }

//     // puts("Press [S] to save default settings or any other key to play", 0xFF0000);
//     printScreen();

//     sleep(START_DELAY);    
// }


// static void tie(){
//     blackOut();
//     puts("It's a tie!", 0xFF0000);
//     printScreen();
// }

// static void lostPlayer1(){
//     blackOut();
//     puts("Player 1 wins", 0xFF0000);
//     printScreen();
// }

// static void lostPlayer2(){
//     blackOut();
//     puts("Player 2 wins", 0xFF0000);
//     printScreen();
// }

// static void setBorder(){
//   for(int i = 0; i < WIDTH; i++){
//     occupied[0][i] = 1;
//     occupied[HEIGHT - 1][i] = 0;
//   }
//   for(int i = 1; i < HEIGHT - 1; i++){
//     occupied[i][0] = 1;
//     occupied[i][WIDTH - 1] = 0;
//   }
// }

// void eliminator() {
//     clearScreen();
//     menu();
//     clearScreen();
//     setBorder();
//     //arranca el juego
//     char lastKeyPlayer1 = 0, lastKeyPlayer2 = 0;
//     char players = 2;
//     int x1 = WIDTH / 2, y1 = START_OFFSET, x2 = WIDTH / 2 + 1, y2 = HEIGHT - START_OFFSET;
//     char finished = 0;
//     while(!finished){
//         // Check Tie
//         if(occupied[x1][y1] != 0 && occupied[x2][y2] != 0) { 
//             tie();
//             finished = 1;
//         }
//         // Check Player 1 loses
//         if(occupied[x1][y1] != 0) { 
//             lostPlayer1();
//             finished = 1;
//         }
//         // Check Player 2 los
//         if(occupied[x2][y2] != 0) {
//             lostPlayer2();
//             finished = 1;
//         }
        
//         // Update occupied position
//         occupied[x1][y1] = 1;
  
        
//         // Get keyboard inputs
//         char c = 1;
//         while(c) {
//             getChar(&c);
//             if(c >= 'A' || c <= 'Z') c = c - 'A' + 'a';
//             switch(c) {
//                 case 'w':
//                 case 'a':
//                 case 's': 
//                 case 'd':
//                   lastKeyPlayer1 = c;
//                   break;
                  
//                 case 'i':
//                 case 'j':
//                 case 'k': 
//                 case 'l':
//                   lastKeyPlayer2 = c;
//                   break;
//             }
//         }

//         // Move player 1 
//         switch(lastKeyPlayer1) {
//             case 'w': 
//                 y1--;
//                 break;
//             case 'a':
//                 x2--;
//                 break;
//             case 's':
//                 y1++;
//                 break;
//             case 'd':
//                 x1++;
//                 break;
//         }
        
        
//         // Print new position player 1
//         putSquare(PLAYER_1_COLOR, x1, y1);
        
//         if(players == 2) {
//             // Update occupied position
//             occupied[x2][y2] = 1;
            
//             // Update direction player 2
//             switch(lastKeyPlayer2) {
//                 case 'i':
//                     y2--;
//                     break;
//                 case 'j':
//                     x2--;
//                     break;
//                 case 'k':
//                     y2++;
//                     break;
//                 case 'l':
//                     x2++;
//                     break;
//             }

//             // Print new position player 2
//             putSquare(PLAYER_2_COLOR, x2, y2);
//         }

//         //sleep(speed);
//     }     
// }