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

int main (int argc, char* arv[]) {

    FILE* fpt;
    char temp[15];
    char **availableTerms = (char**)malloc(140);
    int itemCount = 0;

    // for printing current date and time
    time_t t;
    time(&t);

    // printf("i'm parent number %d\n",getpid());

    // get opened terminals
    for (int i = 0; i <= 136; i++) {
        sprintf(temp, "/dev/pts/%d", i);
        if (fopen(temp, "r") != NULL) {
            availableTerms[itemCount] = (char*)malloc(strlen(temp) + 1);
            strcpy(availableTerms[itemCount], temp);
            itemCount++;
        }
    }

    // make sure enoguh terminals are open
    if (itemCount < 4) {
        printf("error: functionality requires at least 4 terminals total.\n");
    }

    availableTerms = realloc(availableTerms, itemCount*sizeof(char*));

    printf("START DATE OF MISSION : %s", ctime(&t));

    if (fork() != 0){
        if (fork() != 0){
            if (fork() != 0){

            }
            else 
        }
        else
    }
    else 



    return 0;
}
