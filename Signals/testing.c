#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>


int storage;

void alarmHandler(int signum){
    printf("Current storage: %d\n", storage);
}

void user1Handler(int signum){
    printf("\ndecrementing storage...\n");
    storage -= 1;

}
void main(){
    signal(SIGALRM, alarmHandler);
    signal(SIGUSR1, user1Handler);
    
    int parent = getpid();
    int input;
    int child = fork();
    storage = 10;

    if (child == 0){
        for (int i = 0; i < 2; i++)
            sleep(1);
        printf("child process is %d\n", getpid());

        exit(EXIT_FAILURE);
    }
    
    int waitpid = wait(&input);
    if(WIFEXITED(input))
        printf("all good: code %d\n", WEXITSTATUS(input));

}