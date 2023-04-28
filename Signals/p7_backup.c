/*
Signals Submarine Game
Jai Agarwal
ECE 2220 Prog 7
Purpose        - launch submarines (child processes) that print on other terminals
Assumptions    - requires 4 total terminal windows open
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
int global_count;

// child operation
void spawnSub(char *Terminal);

// handle the alarm for 1, 2, 3 sec
void alarmHandler(int signum);

void printReport(FILE* outFile);

int main (int argc, char* arv[]) {
    FILE* outFile;
    char temp[15];
    char **availableTerms = (char**)malloc(140);
    int itemCount = 0;
    signal(SIGALRM, (fptr)alarmHandler);

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
    fclose(outFile);


    if (fork() != 0){
        if (fork() != 0){
            if (fork() != 0){
               // do  something 
            }
            else spawnSub(availableTerms[2]); 
        }
        else spawnSub(availableTerms[1]);
    }
    else spawnSub(availableTerms[0]);

    return 0;
}


void spawnSub(char *Terminal) {
    FILE* outFile = fopen(Terminal, "w");
    srand(time(0) + getpid());

    // initialize distance from base to 0
    SUB.distance = 0;

    // initialize fuel between 1000 and 5000
    SUB.fuel = (rand() % (5000 - 1000 + 1)) + 1000;

    // initialize payload between 6 and 10 missiles
    SUB.payload = (rand() % (10 - 6 + 1)) + 6;
    
    global_fpt = outFile;
    global_count = 0;

    // use alarm()
    while(SUB.fuel > 0){
        alarm(1);
        ++global_count;
        pause();
        // fprintf(outFile, "I have %d gallons of fuel\n", SUB.fuel);
    }
    fprintf(outFile, "I ran out out fuel!!!\n");
    
    fclose(outFile);
}

void alarmHandler(int signum) {
    // - decrement fuel by num between 100 and 200 each sec
    // - increment distance by 5 to 10 every two seconds if going towards
    //     and decrements distance by 3 to 8 if returning to base
    // - report military time, fuel amount, missiles left, and distance from base every 3 seconds
    //     in each terminal

    int towardsBase = 1;

    SUB.fuel -= (rand() % (200 - 100 + 1)) + 100;

    if (global_count == 2){
        if (towardsBase)
            SUB.distance += (rand() % (10 - 5 + 1)) + 5;
        else
            SUB.distance += (rand() % (8 - 3 + 1)) + 3;
    }

    if (global_count == 3){
        printReport(global_fpt);
        global_count = 0;
    }
}




void printReport(FILE* outFile){
    time_t raw_time;
    struct tm *time_ptr;
    char timeString[30];

    time(&raw_time);
    time_ptr = localtime(&raw_time);
    strftime(timeString, 30, "%H:%M:%S", time_ptr);

    fprintf(outFile, "\033[H\033[J"); // clear terminal
    fprintf(outFile, "-------I am sub %d-------\n", getpid());
    fprintf(outFile, "Current time:  %s\n", timeString);
    fprintf(outFile, "\tI am %d miles away from base\n", SUB.distance);
    fprintf(outFile, "\tI have %d ballistic missiles\n", SUB.payload);
    fprintf(outFile, "\tI have %d gallons of fuel\n", SUB.fuel);
}
