//tic-tac-toe game
// Jai Agarwal
// ECE 2220
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// function for printing a 2D array
void printBoard(char array[][3]){
    for(int i=0; i<3; i++){
        printf("\t\t-------------\n");
        printf("\t\t");
        for(int j=0; j<3; j++)
            printf("| %c ", array[i][j]);
        printf("|\n");
    }
    printf("\t\t-------------\n");
}

int main(){

    //declare a matrix that acts as the board/grid
    char board[3][3];
    int userChoice = 0, compChoice = 0, winner = 0, playAgain = 1, userWin = 0, compWin = 0;
    srand(time(0)); //seed the rand function

    printf("\t Welcome to Tic-Tac-Clemson\n");

    while (playAgain == 1){
        //reset things, reinitialize the board matrix 
        winner = 0;
        char j = '1';
        for(int i=0; i<9; i++)
            board[0][i] = j++;
        printBoard(board);

        // winner = 1 is for user, winner = 2 is for computer, winner = 3 is a tie
        while (winner == 0){
            printf("Choose a Square (enter 1-9): ");

            // if an integer is not entered it will read in a char             
            if (scanf("%d", &userChoice) == 0){
                if (getchar() == 'q'){
                    printf("Terminating program...\n");
                    exit(0);
                } 
            }

            // input validation
            while ((board[0][userChoice-1] == 'C' || board[0][userChoice-1] == 'U')){ 
                printf("That square is taken. Enter another: ");
                if (scanf("%d", &userChoice) == 0){
                    if (getchar() == 'q'){
                        printf("Terminating program...\n");
                        exit(0);   
                    } 
                }
            }
            //set entered square to C
            board[0][userChoice-1] = 'C';

            // row check
            for(int i=0; i<3; i++){
                if (board[i][0] == 'C' && board[i][1] == 'C' && board[i][2] == 'C')
                    winner = 1;
            }

            // column check
            for(int i=0; i<3; i++){
                if (board[0][i] == 'C' && board[1][i] == 'C' && board[2][i] == 'C')
                    winner = 1;
            }

            // diagonal check
            if ((board[0][0] == 'C' && board[1][1] == 'C' && board[2][2] == 'C') || (board[0][2] == 'C' && board[1][1] == 'C' && board[2][0] == 'C') ){
                winner = 1;
            }

            // for a tie, checks each square to see if it is taken and that there is no winner already.
            if((board[0][0] != '1' && board[0][1] != '2' && board[0][2] != '3' && board[1][0] != '4' && board[1][1] != '5' && board[1][2] != '6' && board[2][0] != '7' && board[2][1] != '8' && board[2][2] != '9') && (winner != 1)){
                winner = 3;
            }
            else{
                printBoard(board);
                if (winner != 1){
                    compChoice = rand() % 9 + 1;
                    // input validation for computer
                    while ((board[0][compChoice-1] == 'C' || board[0][compChoice-1] == 'U')){ 
                        compChoice = rand() % 9 + 1; 
                    }
                    //set chosen square to U
                    board[0][compChoice-1] = 'U';

                    printf("Computer chooses %d\n", compChoice);
                    printBoard(board);
                    
                    // row check
                    for(int i=0; i<3; i++){
                        if (board[i][0] == 'U' && board[i][1] == 'U' && board[i][2] == 'U')
                            winner = 2;
                    }

                    // column check
                    for(int i=0; i<3; i++){
                        if (board[0][i] == 'U' && board[1][i] == 'U' && board[2][i] == 'U')
                            winner = 2;
                    }

                    // diagonal check
                    if ((board[0][0] == 'U' && board[1][1] == 'U' && board[2][2] == 'U') || (board[0][2] == 'U' && board[1][1] == 'U' && board[2][0] == 'U') ){
                        winner = 2;
                    }
                }
            }
            
        } // end of round

        // outputs a winner message
        if (winner == 1){
            printf("You Win!\n");
            userWin++;
        }
        else if (winner == 2){
            printf("Computer Wins!\n");
            compWin++;
        }
        else if (winner == 3){
            printf("It is a tie!\n");
        }

        // display current win rates
        printf("Current standings\nYou: %d\nComputer: %d\n", userWin, compWin);
        printf("Hit 1 to play again or q to quit: ");
        if (scanf("%d", &playAgain) == 0){
            if (getchar() == 'q'){
                printf("Terminating program...\n");
                exit(0);   
            } 
        }

    } // end of game loop

    return 0;
    
}