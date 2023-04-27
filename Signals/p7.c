/*
Signals Submarine Game
Jai Agarwal
ECE 2220 Prog 7
Purpose        -
Assumptions    -
Known Bugs     -    
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

struct SUB{ int distance, fuel, payload;};

// child operation
void spawnSub(char *Terminal);

int main (int argc, char* arv[]) {

    FILE* fptr;
    char temp[15];
    char **availableTerms = (char**)malloc(140);
    int itemCount = 0;

    // for printing current date and time
    time_t t;
    time(&t);

    // store this current tty as the parent tty    
    char parentTTY[15];
    strcpy(parentTTY, ttyname(STDOUT_FILENO));


    // get opened terminals
    for (int i = 0; i <= 136; i++) {
        sprintf(temp, "/dev/pts/%d", i);
        if (strcmp(temp, parentTTY) && fopen(temp, "r") != NULL) { // I want to keep parentTTY separately
            availableTerms[itemCount] = (char*)malloc(strlen(temp) + 1);
            strcpy(availableTerms[itemCount], temp);
            itemCount++;
        }
    }

    // make sure enough terminals are open
    if (itemCount < 3) {
        printf("error: functionality requires at least 4 terminals total.\n");
        exit(1);
    }

    availableTerms = realloc(availableTerms, itemCount*sizeof(char*));

    fptr = fopen(parentTTY, "w");
    fprintf(fptr, "START DATE OF MISSION : %s\n", ctime(&t));


    if (fork() != 0){
        if (fork() != 0){
            if (fork() != 0){
                
            }
            else spawnSub(availableTerms[0]); 
        }
        else spawnSub(availableTerms[1]);
    }
    else spawnSub(availableTerms[2]);

    return 0;
}


void spawnSub(char *Terminal) {
    struct SUB localSub;

    FILE* fptr = fopen(Terminal, "w");
    srand(time(0) + getpid());

    fprintf(fptr, "\033[H\033[J");


    // initialize distance from base to 0
    localSub.distance = 0;

    // initialize fuel between 1000 and 5000
    localSub.fuel = (rand() % (5000 - 1000 + 1)) + 1000;

    // initialize payload between 6 and 10 missiles
    localSub.payload = (rand() % (10 - 6 + 1)) + 6;

    fprintf(fptr, "I am local sub %d\n", getpid());
    fprintf(fptr, "I am %d miles away from base\n", localSub.distance);
    fprintf(fptr, "I have %d gallons of fuel\n", localSub.fuel);
    fprintf(fptr, "I have %d ballistic missiles\n", localSub.payload);


    /*  use alarm() or setitimer and then:
    
        - decrement fuel by num between 100 and 200 each sec
        - increment distance by 5 to 10 every two seconds if going towards
            and decrements distance by 3 to 8 if returning to base
        - report military time, fuel amount, missiles left, and distance from base every 3 seconds
            in each terminal
    */


    
    fclose(fptr);
}

