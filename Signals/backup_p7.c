/*
Signals Submarine Game
Jai Agarwal
ECE 2220 Prog 7
Purpose        - launch submarines (child processes) that print on other terminals
Assumptions    - requires 4 total terminal windows open
               - requires the user has all possible terminals in front of them
                  due to how the program gets the terminals in order 
Known Bugs     -    
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>


// GLOBALS
struct SUB{int distance, fuel, payload;}SUB;
typedef void (*fptr)();
FILE* global_fpt;
int global_count, global_returningToBase, _SUCCESS = 0;
int PARENTPID;

// child operation
void spawnSub(char *Terminal);

// handle the alarm for 1, 2, 3 sec
void alarmHandler(int signum);

void sigUserHandler1(int signum);

void sigUserHandler2(int signum);

void successHandler(int signum);

void killHandler(int signum);

void printReport();

int main (int argc, char* arv[]) {
    FILE* baseTerminal;
    char tempRead[15];
    char **availableTerms = (char**)malloc(140);
    int itemCount = 0;
    char parentTTY[15], parentInput[3];
    int SUB1, SUB2, SUB3;
    // int SUB1status, SUB2status, SUB3status;
    int status1 = 0, status2 = 0, status3 = 0;

    signal(SIGUSR1, (fptr)sigUserHandler1);
    signal(SIGUSR2, (fptr)sigUserHandler2);
    signal(SIGTERM, (fptr)successHandler);
    signal(SIGABRT, (fptr)killHandler);
    // signal(SIGFPE, (fptr)parentWaitingHandler);

    // for printing current date and time
    time_t t;
    time(&t);

    // store this current tty as the parent tty    
    strcpy(parentTTY, ttyname(STDOUT_FILENO));

    // to store parent pid for later
    PARENTPID = getpid();

    // get opened terminals
    for (int i = 0; i <= 136; i++) {
        sprintf(tempRead, "/dev/pts/%d", i);
        if (strcmp(tempRead, parentTTY) && fopen(tempRead, "r") != NULL) { // I want to keep parentTTY separately
            availableTerms[itemCount] = (char*)malloc(strlen(tempRead) + 1);
            strcpy(availableTerms[itemCount], tempRead);
            itemCount++;
        }
    }

    // make sure enough terminals are open
    if (itemCount < 3) {
        printf("error: functionality requires at least 4 terminals total.\n");
        exit(1);
    }

    availableTerms = realloc(availableTerms, itemCount*sizeof(char*));

    if (fork() != 0){
        if (fork() != 0){
            if (fork() != 0){

            }
            else spawnSub(availableTerms[2]); 
        }
        else spawnSub(availableTerms[1]);
    }
    else spawnSub(availableTerms[0]);

    SUB1 = PARENTPID + 1;
    SUB2 = PARENTPID + 2;
    SUB3 = PARENTPID + 3;

    if (fork() == 0) {

        // prints to the parent tty stdout
        baseTerminal = fopen(parentTTY, "w");
        fprintf(baseTerminal, "\033[H\033[J"); // clear terminal
        fprintf(baseTerminal, "START DATE OF MISSION : %s\n", ctime(&t));
        fprintf(baseTerminal, "ln - launch missile from sub 'n'\nrn - refuel sub 'n'\n");
        fprintf(baseTerminal, "sn - scuttle sub 'n'\nq - exit mission\n");
        
        while(1) {

            // if (WIFEXITED(status1) && WIFEXITED(status2) && WIFEXITED(status3)){
            //     missionReport(status1, status2, status3);
            // }
            scanf("%s", parentInput);

            if (parentInput[0] == 113){
                kill(SUB1, SIGKILL);
                kill(SUB2, SIGKILL);
                kill(SUB3, SIGKILL);
                printf("SESSION ENDED.\n");

                for (int i = 0; i < itemCount; i++) free(availableTerms[i]);
                free(availableTerms);
                exit(0);
            }

            switch (parentInput[0]){
                case 108:   // launch missile
                    if (parentInput[1] == 49) kill(SUB1, SIGUSR1);
                    else if (parentInput[1] == 50) kill(SUB2, SIGUSR1);
                    else if (parentInput[1] == 51) kill(SUB3, SIGUSR1);
                    else {
                        printf("improper input: \n");
                        break;
                    }
                    printf("Launching from SUB%c\n", parentInput[1]);
                    break;
                case 114:   // refuel sub
                    if (parentInput[1] == 49) kill(SUB1, SIGUSR2);
                    else if (parentInput[1] == 50) kill(SUB2, SIGUSR2);
                    else if (parentInput[1] == 51) kill(SUB3, SIGUSR2);
                    else {
                        printf("improper input: \n");
                        break;
                    }
                    printf("Refueling SUB%c\n", parentInput[1]);
                    break;
                case 115:   // scuttle sub
                    if (parentInput[1] == 49) kill(SUB1, SIGABRT);
                    else if (parentInput[1] == 50) kill(SUB2, SIGABRT);
                    else if (parentInput[1] == 51) kill(SUB3, SIGABRT);
                    else {
                        printf("improper input: \n");
                        break;
                    }
                    printf("SUB%c scuttled.\n", parentInput[1]);
                    break;
            }
        }
    }

    waitpid(SUB1, &status1, 0);
    waitpid(SUB2, &status2, 0);
    waitpid(SUB3, &status3, 0);

    // if all of children have exited
    if (WIFEXITED(status1) && WIFEXITED(status2) && WIFEXITED(status3)) {
        time(&t);
        for (int i = 0; i < itemCount; i++) free(availableTerms[i]);
        free(availableTerms);

        printf("\n-------------------------\n%sMISSION REPORT:\n", ctime(&t));
        WEXITSTATUS(status1) ? printf("SUB1 FAILED\n") : printf("SUB1 SUCCEEDED\n");
        WEXITSTATUS(status2) ? printf("SUB2 FAILED\n") : printf("SUB2 SUCCEEDED\n");
        WEXITSTATUS(status3) ? printf("SUB3 FAILED\n") : printf("SUB3 SUCCEEDED\n");
        printf("-------SESSION DONE-------\n");
        exit(0);
    }


    // // signal(SIGUSR2, (fptr)parentRescueSignal);
    // SUB1 = PARENTPID + 1;
    // SUB2 = PARENTPID + 2;
    // SUB3 = PARENTPID + 3;

    if (getpid() == PARENTPID) {
        // // prints to the parent tty stdout
        // printf("\033[H\033[J"); // clear terminal
        // printf("START DATE OF MISSION : %s\n", ctime(&t));
        // printf("ln - launch missile from sub 'n'\nrn - refuel sub 'n'\n");
        // printf("sn - scuttle sub 'n'\nq - exit mission\n");
        
        // while(1) {

        //     // if (WIFEXITED(status1) && WIFEXITED(status2) && WIFEXITED(status3)){
        //     //     missionReport(status1, status2, status3);
        //     // }
        //     scanf("%s", parentInput);

        //     if (parentInput[0] == 113){
        //         kill(SUB1, SIGKILL);
        //         kill(SUB2, SIGKILL);
        //         kill(SUB3, SIGKILL);
        //         printf("SESSION ENDED.\n");

        //         for (int i = 0; i < itemCount; i++) free(availableTerms[i]);
        //         free(availableTerms);
        //         exit(0);
        //     }

        //     switch (parentInput[0]){
        //         case 108:   // launch missile
        //             if (parentInput[1] == 49) kill(SUB1, SIGUSR1);
        //             else if (parentInput[1] == 50) kill(SUB2, SIGUSR1);
        //             else if (parentInput[1] == 51) kill(SUB3, SIGUSR1);
        //             printf("Launching from SUB%c\n", parentInput[1]);
        //             break;
        //         case 114:   // refuel sub
        //             if (parentInput[1] == 49) kill(SUB1, SIGUSR2);
        //             else if (parentInput[1] == 50) kill(SUB2, SIGUSR2);
        //             else if (parentInput[1] == 51) kill(SUB3, SIGUSR2);
        //             printf("Refueling SUB%c\n", parentInput[1]);
        //             break;
        //         case 115:   // scuttle sub
        //             if (parentInput[1] == 49) kill(SUB1, SIGABRT);
        //             else if (parentInput[1] == 50) kill(SUB2, SIGABRT);
        //             else if (parentInput[1] == 51) kill(SUB3, SIGABRT);
        //             printf("SUB%c scuttled.\n", parentInput[1]);
        //             break;
        //     }
        // }
    }

    return 0;
}


void spawnSub(char *Terminal) {
    // open terminal for writing and clear its conents
    FILE* outFile = fopen(Terminal, "w");
    fprintf(outFile, "\033[H\033[J"); // clear terminal

    signal(SIGALRM, (fptr)alarmHandler);
    srand(time(0) + getpid());

    // initialize distance from base to 0
    SUB.distance = 0;

    // rand() % (max - low + 1) + low
    // initialize fuel between 1000 and 5000
    SUB.fuel = rand() % (5000 - 1000 + 1) + 1000;

    // initialize payload between 6 and 10 missiles
    SUB.payload = rand() % (10 - 6 + 1) + 6;
    
    global_fpt = outFile;
    global_count = 0;

    // use alarm()
    while(SUB.fuel > 0){
        alarm(1);
        ++global_count;
        pause();
    
        if (global_returningToBase && SUB.distance <= 0){
            _SUCCESS = 1;
            kill(getpid(), SIGTERM);
        }
    }

    kill(getpid(), SIGUSR2);
    kill(getpid(), SIGTERM);
}


void alarmHandler(int signum) {
    SUB.fuel -= rand() % (200 - 100 + 1) + 100;

    if (global_count % 2 == 0){
        if (global_returningToBase)
            SUB.distance -= rand() % (8 - 3 + 1) + 3;
        else
            SUB.distance += rand() % (10 - 5 + 1) + 5;
    }

    if (global_count % 3 == 0){
        printReport();
    }

    signal(SIGALRM, (fptr)alarmHandler);
}


// user1 handler
void sigUserHandler1(int signum) {
    if (SUB.payload-1 >= 0) {
        fprintf(global_fpt, "Launching missile!\n");
        SUB.payload -= 1;
    }

    if (SUB.payload == 0) {
        fprintf(global_fpt, "No more missiles, returning to base!\n");
        global_returningToBase = 1;
    }
    else fprintf(global_fpt, "%d missiles left.\n", SUB.payload);

    signal(SIGALRM, (fptr)alarmHandler);
}


// user2 handler
void sigUserHandler2(int signum) {
    if (SUB.fuel <= 0) {
        fprintf(global_fpt, "Help is on the way!\n");
    }
    else {
        fprintf(global_fpt, "refueling!\n");
        SUB.fuel = rand() % (5000 - 1000 + 1) + 1000;
        fprintf(global_fpt, "Refueled to %d.\n", SUB.fuel);
    }

    signal(SIGALRM, (fptr)alarmHandler);
}

// success handler (SIGTERM)
void successHandler(int signum) {
    if (_SUCCESS) {
        fprintf(global_fpt, "MISSION SUCCESS\n");
        exit(EXIT_SUCCESS);
    }
    else {
        printf("SUB%d DEAD IN THE WATER\n", getpid() - PARENTPID);
        exit(EXIT_FAILURE);
    }
}


// kill handler
void killHandler(int signum) {
    fprintf(global_fpt, "You have been scuttled!\n");
    exit(1);
}

void printReport() {
    time_t raw_time;
    char timeString[30];

    time(&raw_time);
    strftime(timeString, 30, "%H:%M:%S", localtime(&raw_time));

    fprintf(global_fpt, "\033[H\033[J"); // clear terminal
    fprintf(global_fpt, "\n-------I am sub %d-------\n", getpid() - PARENTPID);
    fprintf(global_fpt, "Current time:  %s\n", timeString);
    fprintf(global_fpt, "\tI am %d miles away from base\n", SUB.distance);
    fprintf(global_fpt, "\tI have %d ballistic missiles\n", SUB.payload);
    fprintf(global_fpt, "\tI have %d gallons of fuel\n", SUB.fuel);
    if (SUB.fuel < 500) fprintf(global_fpt, "I am running out of fuel!\n");

    
}