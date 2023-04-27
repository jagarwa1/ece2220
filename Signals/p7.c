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
#include <signal.h>
#include <time.h>


// GLOBALS
struct SUB{int distance, fuel, payload;}SUB;
typedef void (*fptr)();
FILE* global_fpt;

// child operation
void spawnSub(char *Terminal);

// handle the alarm
void alarmHandler(int signum);

void printReport();

int main (int argc, char* arv[]) {
    FILE* outFile;
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

    outFile = fopen(parentTTY, "w");
    fprintf(outFile, "START DATE OF MISSION : %s\n", ctime(&t));


    if (fork() != 0){
        if (fork() != 0){
            if (fork() != 0){
               // do  something 
            }
            else spawnSub(availableTerms[0]); 
        }
        else spawnSub(availableTerms[1]);
    }
    else spawnSub(availableTerms[2]);

    return 0;
}


void spawnSub(char *Terminal) {
    FILE* outFile = fopen(Terminal, "w");
    srand(time(0) + getpid());
    signal(SIGALRM, (fptr)alarmHandler);

    // initialize distance from base to 0
    SUB.distance = 0;

    // initialize fuel between 1000 and 5000
    SUB.fuel = (rand() % (5000 - 1000 + 1)) + 1000;

    // initialize payload between 6 and 10 missiles
    SUB.payload = (rand() % (10 - 6 + 1)) + 6;
    
    global_fpt = outFile;

    // use alarm()
    while(SUB.fuel > 0){
        alarm(3);
        pause();
    }
    fprintf(global_fpt, "I ran out out fuel!!!\n");

    
    fclose(outFile);
}

void alarmHandler(int signum){
    // - decrement fuel by num between 100 and 200 each sec
    // - increment distance by 5 to 10 every two seconds if going towards
    //     and decrements distance by 3 to 8 if returning to base
    // - report military time, fuel amount, missiles left, and distance from base every 3 seconds
    //     in each terminal

    for (int i = 0; i < 3; i++) SUB.fuel -= (rand() % (200 - 100)) + 100;
    for (int i = 0; i < 2; i++) SUB.distance += (rand() % (10 - 5)) + 5;
    printReport();    

}


void printReport(){
    time_t raw_time;
    struct tm *time_ptr;
    char timeString[30];

    time(&raw_time);
    time_ptr = localtime(&raw_time);
    strftime(timeString, 30, "%H:%M:%S", time_ptr);
    
    
    fprintf(global_fpt, "\033[H\033[J"); // clear terminal
    fprintf(global_fpt, "Current time:  %s\n", timeString);
    fprintf(global_fpt, "-------I am sub %d-------\n", getpid());
    fprintf(global_fpt, "I am %d miles away from base\n", SUB.distance);
    fprintf(global_fpt, "I have %d ballistic missiles\n", SUB.payload);
    fprintf(global_fpt, "I have %d gallons of fuel\n", SUB.fuel);
}
