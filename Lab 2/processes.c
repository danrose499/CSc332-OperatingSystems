#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int child1 = fork();
    int waitStatus[2];
    if(child1 == 0) { //Code for first child
        printf("\nI am first child, my pid is: %d\n", getpid());
        wait(&waitStatus[0]);
    }
    else { //Code for parent
        int child2 = fork();
        if(child2 == 0){ //Code for second child
            printf("\nI am second child, my pid is: %d\n", getpid());
            wait(&waitStatus[1]);
        }
    }
    return 0;
}