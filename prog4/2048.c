// 2048 Game Using Curses
// Jai Agarwal
// Program 4 ECE 2220
// Assumptions: - assumes that the user has ncurses.h installed
//              - assumes that the user compiles with -lncurses                
// Known Bugs:  - after continuing after a 2048 tile win it has segfaulted or frozen when the board gets full. if frozen, then ctrl+c has stopped it.

#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

// ARGS: takes a WINDOW* gameboard, WINDOW* topLabel, 2D grid board, a bool turn-checking variable, int current score, and int high score
// RETURNS: none
// PURPOSE: creates a new window and writes the gameboard information into it
void printBoard(WINDOW* gameBoard, WINDOW* topLabel, int gameboard[4][4], bool valid, int score, int high);

// ARGS:    takes a 2D array and int score variable
// RETURNS: returns boolean variable true if a move occurred
// PURPOSE: shift board left
bool moveLeft(int board[4][4], int *score);

// ARGS:    takes a 2D array and int score variable
// RETURNS: returns boolean variable true if a move occurred
// PURPOSE: shift board right
bool moveRight(int board[4][4], int *score);

// ARGS:    takes a 2D array and int score variable
// RETURNS: returns boolean variable true if a move occurred
// PURPOSE: shift board down
bool moveDown(int board[4][4], int *score);

// ARGS:    takes a 2D array and int score variable
// RETURNS: returns boolean variable true if a move occurred
// PURPOSE: shift board up
bool moveUp(int board[4][4], int *score);

// ARGS:   takes 2D gameboard
// RETURNS: returns boolean variable true if game is over
// PURPOSE: check for any empty cells and any tile pairs
bool gameOver(int board[4][4]);

// ARGS:    that takes a 2D gameboard
// RETURNS: a boolean variable true if the game is won and false if not
// PURPOSE: check for 2048 tile
bool gameWon(int board[4][4]);

void chooseColor(int, WINDOW*);

int main(){	
    curs_set(0);
    initscr();			    // start curses mode
    cbreak();			    // get input immediately and allow for interrupts
    noecho();               // don't echo what is inputted
    keypad(stdscr, true);   // get key inputs
    start_color();			// colors


    int inputDir, playAgain;
    bool valid = true;
    int score = 0, high = 0;
    bool haveWon = false;

    int grid[4][4];
    for (int i = 0; i < 16; i++)
        grid[0][i] = 0;

    // put a random number (does it before printBoard call so 2 numbers are generated for the initial round)
    int x = 0, y = 0;
    srand(time(NULL));
    x = rand() % 16;
    while (grid[0][x] != 0){
        x = rand()%16;
    }
    y = (rand() % 10 + 1) <= 9 ? 2 : 4;     // will put a 2 90% of the time and a 4 10% of the time
    grid[0][x] = y;


    attron(A_BOLD);
    mvprintw(2, 18, "2048");
    move(2, 23); // get the cursor out of the way
    attroff(A_BOLD);
    refresh();

    // various windows that will be used
    WINDOW* gameBoard = newwin(10, 35, 5, 4);
    wrefresh(gameBoard);
    
    WINDOW* label = newwin(3, 35, 15, 6);
    wprintw(label, "use arrow keys, press q to exit");
    wrefresh(label);

    WINDOW* lowestLabel = newwin(1, 35, 17, 7);
    wrefresh(lowestLabel);

    WINDOW* topLabel = newwin(3, 50, 4, 4);
    // print initial board
    printBoard(gameBoard, topLabel, grid, valid, score, high);

    while((inputDir = getch()) != 113){ // while the user does not enter a q, loop

        move(2, 23); // get the cursor out of the way

        while (!(inputDir == KEY_LEFT || inputDir == KEY_RIGHT || inputDir == KEY_UP || inputDir == KEY_DOWN)){
            inputDir = getch();
        }

        switch(inputDir){	

            case KEY_LEFT: 		
            valid = moveLeft(grid, &score);		
            break;
        
            case KEY_RIGHT:
            valid = moveRight(grid, &score);
            break;

            case KEY_UP:
            valid = moveUp(grid, &score);
            break;

            case KEY_DOWN:
            valid = moveDown(grid, &score);
            break;	
        }

        printBoard(gameBoard, topLabel, grid, valid, score, high);

        if (valid){
            // if they have not won AND there is a 2048 tile on the board
            if (!haveWon && gameWon(grid)){

                    //display win message and get input
                    werase(label);
                    wprintw(label, "YOU WIN! Keep going? y/n: ");
                    wrefresh(label);
                    playAgain = getch();

                    while (!(playAgain == 121 || playAgain == 110)){    // input validation for playAgain
                        wprintw(lowestLabel, "invalid, enter again: ");
                        wrefresh(lowestLabel);
                        playAgain = getch();
                        werase(lowestLabel);
                    }

                    if(playAgain == 121){ // if they want to play again, record that they have won and then reprint the board
                        haveWon = true;
                        werase(label);
                        wprintw(label, "use arrow keys, press q to exit");
                        wrefresh(label);
                        printBoard(gameBoard, topLabel, grid, valid, score, high); // reprint board for start of new game
                    }
                    // if they do not want to play again
                    else if(playAgain == 110){ // if no new game then exit
                        endwin();
                        printf("Score: %d   High: %d \nExiting...\n", score, high);
                        exit(1);
                    }
                }

            // if the game is over, ask if they want to play again
            if (gameOver(grid)){

                // display game over and get input
                werase(label);
                wprintw(label, "GAME OVER! Play again? y/n: ");
                wrefresh(label);	
                playAgain = getch();

                while (!(playAgain == 121 || playAgain == 110)){        // input validation for playAgain
                    wprintw(lowestLabel, "invalid, enter again: ");
                    wrefresh(lowestLabel);
                    playAgain = getch();
                    werase(lowestLabel);
                }

                // if they want to play again, reset the grid
                if(playAgain == 121){ 

                    for(int i=0; i<16; i++)
                        grid[0][i] = 0;
                    srand(time(NULL));
                    x = rand() % 16;
                    while (grid[0][x] != 0)
                        x = rand()%16;
                    y = (rand() % 10 + 1) <= 9 ? 2 : 4;     // put a 2 90% of the time and a 4 10% of the time
                    grid[0][x] = y;
                    
                    // set new high score
                    if (score > high)
                        high = score; // record their high score
                    score = 0;      // reset score to 0

                    // put default label back
                    werase(label);
                    wprintw(label, "use arrow keys, press q to exit");
                    wrefresh(label);
                    
                    // reprint board for start of new game with new high score
                    printBoard(gameBoard, topLabel, grid, valid, score, high);

                }
                // if they do not want to play again
                else if (playAgain == 110) { // display exit message and then exit program
                    endwin();
                    printf("Score: %d   High: %d \nExiting...\n", score, high);
                    exit(1);
                }
            }
        } // end of if (valid)

    }// end of while loop
    endwin();
    printf("Score: %d   High: %d \nExiting...\n", score, high);
    exit(1);
    return 0;
}// end of main

void printBoard(WINDOW* gameBoard, WINDOW* topLabel, int grid[4][4], bool valid, int score, int high){	

    init_pair(1, COLOR_BLACK, COLOR_RED);

    // generate a random tile
    int x = 0, y = 0;
    if (valid){
        srand(time(NULL));
        x = rand() % 16;
        while (grid[0][x] != 0){
            x = rand()%16;
        }
        y = (rand() % 10 + 1) <= 9 ? 2 : 4;     // will put a 2 90% of the time and a 4 10% of the time
        grid[0][x] = y;
    }

    // rewrite top label to display score
    werase(topLabel);
    wprintw(topLabel, " Score: %d\t   High Score: %d\n", score, high);
    wrefresh(topLabel);	

    werase(gameBoard);
    // refill the gameboard
    for(int i = 0; i < 4; i++){
        wprintw(gameBoard, "\n  ");
        wrefresh(gameBoard);
        for(int j = 0; j < 4; j++){
            chooseColor(grid[i][j], gameBoard);
        }
        wprintw(gameBoard, "\n");
        wrefresh(gameBoard);
    }
    box(gameBoard, 0, 0);
    wrefresh(gameBoard);
}

bool moveRight(int board[4][4], int *score){
    bool valid = false;
    int temp;
    int col;
    for(int r = 0; r < 4; r++){
        //slide over
        for(int c = 3; c > 0; c--){
            if (board[r][c] == 0){
                col = c;
                while(board[r][col] == 0 && col > 0){
                    col--;
                }
                if(board[r][col] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[r][col];
                board[r][col] = temp;
            }
        }
        // add like terms
        for(int c = 3; c > 0; c--){ 
            if (board[r][c] != 0 && board[r][c] == board[r][c-1]){
                board[r][c] *= 2;
                *score += board[r][c]; 
                board[r][c-1] = 0;
                valid = true;
            }
        }  
        // slide over again
        for(int c = 3; c > 0; c--){
            if (board[r][c] == 0){
                col = c;
                while(board[r][col] == 0 && col > 0){
                    col--;
                }
                if(board[r][col] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[r][col];
                board[r][col] = temp;
            }
        }    
    }
    return valid;
}

bool moveLeft(int board[4][4], int *score){
    bool valid = false; // boolean variable to record if a move happened
    int temp;
    int col;
    for(int r = 0; r < 4; r++){
        //slide over
        for(int c = 0; c < 4; c++){
            if (board[r][c] == 0){
                col = c;
                while(board[r][col] == 0 && col < 3){
                    col++;
                }
                if(board[r][col] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[r][col];
                board[r][col] = temp;
            }
        }
        // add like terms
        for(int c = 0; c < 3; c++){ 
            if (board[r][c] != 0 && board[r][c] == board[r][c+1]){
                board[r][c] *= 2;
                *score += board[r][c]; 
                board[r][c+1] = 0;
                valid = true;
            }
        }
        // check for slide over again
        for(int c = 0; c < 4; c++){
            if (board[r][c] == 0){
                col = c;
                while(board[r][col] == 0 && col < 3){
                    col++;
                }
                if(board[r][col] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[r][col];
                board[r][col] = temp;
            }
        }
    }
    return valid;
}

bool moveDown(int board[4][4], int *score){
    bool valid = false;
    int temp;
    int row;
    for(int c = 0; c < 4; c++){
        // slide down
        for(int r = 3; r > 0; r--){
            if (board[r][c] == 0){
                row = r;
                while(board[row][c] == 0 && row > 0){
                    row--;
                }
                if(board[row][c] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[row][c];
                board[row][c] = temp;
            }
        }
        // add like terms
        for(int r = 3; r > 0; r--){ 
            if (board[r][c] != 0 && board[r][c] == board[r-1][c]){
                board[r][c] *= 2;
                *score += board[r][c]; 
                board[r-1][c] = 0;
                valid = true;
            }
        }
        // check for slide down again
        for(int r = 3; r > 0; r--){
            if (board[r][c] == 0){
                row = r;
                while(board[row][c] == 0 && row > 0){
                    row--;
                }
                if (board[row][c] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[row][c];
                board[row][c] = temp;
            }
        }
    }
    return valid;
}

bool moveUp(int board[4][4], int *score){
    bool valid = false;
    int temp;
    int row;
    for(int c = 0; c < 4; c++){
        // slide up
        for(int r = 0; r  < 4; r++){
            if (board[r][c] == 0){
                row = r;
                while(board[row][c] == 0 && row < 3){
                    row++;
                }
                if (board[row][c] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[row][c];
                board[row][c] = temp;
            }
        }
        // add like terms
        for(int r = 0; r < 3; r++){ 
            if (board[r][c] != 0 && board[r][c] == board[r+1][c]){
                board[r][c] *= 2;
                *score += board[r][c]; 
                board[r+1][c] = 0;
                valid = true;
            }
        }
         // check for slide up again
        for(int r = 0; r < 4; r++){
            if (board[r][c] == 0){
                row = r;
                while(board[row][c] == 0 && row < 3){
                    row++;
                }
                if (board[row][c] != 0)
                    valid = true;
                temp = board[r][c];
                board[r][c] = board[row][c];
                board[row][c] = temp;
            }
        }
    }
    return valid;
}

bool gameOver(int board[4][4]){
    for(int r = 0; r < 4; r++){
        for (int c = 0; c < 4; c++){
        // check if there are any empty spots
            if (board[r][c] == 0) 
                return false;
        }
        // check for a pair left and right
        for (int c = 0; c < 3; c++){
            if (board[r][c] == board[r][c+1])
                return false;
        }
    }
    // check for a pair up and down
    for(int c = 0; c < 4; c++){
        for (int r = 0; r < 3; r++){
            if (board[r][c] == board[r+1][c])
                return false;
        }
    }
    return true;
}

bool gameWon(int board[4][4]){
    for (int c = 0; c < 16; c++){
        // check if there is any 2048 tile
        if (board[0][c] == 2048)
            return true;
    }
    return false;
}

void chooseColor(int number, WINDOW* gameBoard){

    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_YELLOW);
    init_pair(8, COLOR_BLACK, COLOR_GREEN);
    init_pair(16, COLOR_BLACK, COLOR_RED);
    init_pair(32, COLOR_BLACK, COLOR_BLUE);
    init_pair(64, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(128, COLOR_BLACK, COLOR_YELLOW);
    init_pair(256, COLOR_BLACK, COLOR_RED);
    init_pair(512, COLOR_BLACK, COLOR_WHITE);
    init_pair(1024, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2048, COLOR_BLACK, COLOR_YELLOW);

    if (number == 0){   // if cell is empty then print a '-'
        wprintw(gameBoard, "%4c\t", '-');
    }
    else { 
        wattron(gameBoard, COLOR_PAIR(number));
        wprintw(gameBoard, "%4d\t", number);
        wattroff(gameBoard, COLOR_PAIR(number));
    }
    wrefresh(gameBoard);

}