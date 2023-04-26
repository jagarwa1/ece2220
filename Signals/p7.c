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

int main (int argc, char* arv[]) {
    int parentID = getpid();
    int retID = fork();

    if (retID != 0) {
        printf("go tigers\n");
        if (fork() == 0)
            printf("i'm child number 3, what should i say?\n");
    }
    else {
        printf("help me\n");
    }


    //printf("parentID: %d\n", parentID);


    return 0;
}
