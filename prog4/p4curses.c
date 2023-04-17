// 2048 Game
// Jai Agarwal
// Program 4 ECE 2220
// Assumptions: assumes that the user has ncurses.h installed
// Known Bugs: 

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// function for printing the board and adding a new tile
void printBoard(WINDOW* local_win, int grid[4][4], int score, bool valid, int high){
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

    for(int i = 0; i < 4; i++){
        wprintw(local_win, "\n  ");
        wrefresh(local_win);
        for(int j = 0; j < 4; j++){
            if (grid[i][j] == 0){
                wprintw(local_win, "%4c\t", '-');
                wrefresh(local_win);
            }
            else { 
                wprintw(local_win, "%4d\t", grid[i][j]);
                wrefresh(local_win);
            }
        }
        wprintw(local_win, "\n");
        wrefresh(local_win);
    }
}

// function to move left, takes in a 2D array and a score variable. Shifts array and updates score.
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

// function to move right, takes in a 2D array and a score variable. Shifts array and updates score.
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

// function to move down, takes in a 2D array and a score variable. Shifts array and updates score.
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

// function to move up, takes in a 2D array and a score variable. Shifts array and updates score.
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

// pass 2D gameboard and returns true if game is over
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

// function that takes a 2D gameboard and checks for the 2048 tile
bool gameWon(int board[4][4]){
    for (int c = 0; c < 16; c++){
        // check if there is any 2048 tile
        if (board[0][c] == 2048)
            return true;
    }
    return false;
}

void destroy_win(WINDOW *local_win){
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_win);
    delwin(local_win);
}

WINDOW *create_newwin(int grid[4][4], int score, bool valid, int high){	
    WINDOW *local_win;

    local_win = newwin(10, 35, 4, 4);
    printBoard(local_win, grid, score, valid, high);

    box(local_win, 0, 0);
    wrefresh(local_win);
    return local_win;
}

int main(){
	initscr();	// Start curses mode
    raw();
    noecho();
    keypad(stdscr, TRUE);		/* I need that nifty F2 	*/

    int grid[4][4];
    int score = 0, high = 0, x, y;
    bool valid = true; // will record if a move was successful/valid
    int inputDir, playAgain;
    bool haveWon = false;   // turns true if the player wins so they can play again and it will stop checking for a win
    


    for(int i=0; i<16; i++)
        grid[0][i] = 0;

    grid[0][0] = 2;
    
    // generating one random tile, the printBoard() in the loop will generate the other. (so there are 2 tiles for round 1 and 1 tile otherwise)
    // srand(time(NULL));
    // x = rand() % 16;
    // while (grid[0][x] != 0)
    //     x = rand()%16;
    // y = (rand() % 10 + 1) <= 9 ? 2 : 4;     // put a 2 90% of the time and a 4 10% of the time
    // grid[0][x] = y;

    printw("Press q to exit");
    refresh();

    WINDOW* boardWindow = create_newwin(grid, score, valid, high);
    
    while ((inputDir = getchar()) != 113){    
        
        move(0,15);

        switch(inputDir){
            case KEY_LEFT:
                valid = moveLeft(grid, &score);
                break;
            case KEY_RIGHT:
                valid = moveRight(grid, &score);
                break;
            case KEY_DOWN:
                valid = moveDown(grid, &score);
                break;
            case KEY_UP:
                valid = moveUp(grid, &score);
                break;
        }

        destroy_win(boardWindow);
        boardWindow = create_newwin(grid, score, valid, high);

        if (valid){
            // if they have not won AND there is a 2048 tile on the board
            if (!haveWon && gameWon(grid)){
                    printw("\n\t\tYOU WIN! Keep going? y/n: ");
                    refresh();
                    playAgain = getch();
                    // while (!(playAgain == 121 || playAgain == 110)){
                    //     printw("invalid, enter again: ");
                    //     refresh();
                    //     playAgain = getch();
                    // }
                    if(playAgain == 121){ // if they want to play again, record that they have won and then reprint the board
                        haveWon = true;
                        destroy_win(boardWindow);
                        boardWindow = create_newwin(grid, score, valid, high); // reprint board for start of new game
                    }
                    // if they do not want to play again
                    else { // if no new game then exit
                        printw("\t\tYour score was: %d\n\t\tExiting...\n", score); 
                        refresh();
                        endwin();
                        exit(1);
                    }
                }
            // if the game is over, ask if they want to play again
            if (gameOver(grid)){
                printw("\n\t   GAME OVER! Play again? y/n: ");
                playAgain = getch();
                // while (!(playAgain == 121 || playAgain == 110)){
                //     printw("invalid, enter again: ");
                //     refresh();
                //     playAgain = getch();
                // }
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
                    
                    high = score; // record their highscore
                    printw("\t\tNEW GAME\n");
                    refresh();
                    score = 0;      // reset score to 0
                    destroy_win(boardWindow);
                    boardWindow = create_newwin(grid, score, valid, high); // reprint board for start of new game with new high score

                }
                // if they do not want to play again
                else { // if no new game then exit program
                    printw("\tYour score was: %d\n\tExiting...\n", score); 
                    refresh();
                    endwin();
                    exit(1);
                }
            }
        }

    }
    printw("\t\tYour score was: %d\n\t\tExiting...\n", score);
    refresh();
    endwin();

    return 0;
}