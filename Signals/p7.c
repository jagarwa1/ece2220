/*
Bitmap Files
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


// child operation
void spawnSub(char *Terminal);

int main (int argc, char* arv[]) {

    FILE* fpt;
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
        if (isatty(temp)) {
            availableTerms[itemCount] = (char*)malloc(strlen(temp) + 1);
            strcpy(availableTerms[itemCount], temp);
            itemCount++;
        }
    }

    // make sure enough terminals are open
    if (itemCount < 4) {
        printf("error: functionality requires at least 4 terminals total.\n");
        exit(1);
    }

    availableTerms = realloc(availableTerms, itemCount*sizeof(char*));

    for (int i = 0; i < itemCount; i++){
        fpt = fopen(availableTerms[i], "w");
        fprintf(fpt, "START DATE OF MISSION : %s", ctime(&t));
    }

    if (fork() != 0){
        if (fork() != 0){
            if (fork() != 0){
                
            }
            else spawnSub(availableTerms[1]); 
        }
        else spawnSub(availableTerms[2]);
    }
    else spawnSub(availableTerms[3]);

    return 0;
}


void spawnSub(char *Terminal) {
    FILE* fptr = fopen(Terminal, "w");
    
    // initialize distance from base to 0
    int distance = 0;

    // initialize fuel between 1000 and 5000
    int fuel = rand() % 5000 + 1000;

    // initialize payload between 6 and 10 missiles
    int payload = rand() % 10 + 6;


    /*  use alarm() or setitimer and then:
    
        - decrement fuel by num between 100 and 200 each sec
        - increment distance by 5 to 10 every two seconds if going towards
            and decrements distance by 3 to 8 if returning to base
        - reports military time, fuel amount, missiles left, and distance from base every 3 seconds
    
    */
    
    fclose(fptr);
}

