#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <sys/wait.h>
#include <sys/types.h>

int returnError(char c);
int main(int argc, char *argv[]){

    int p[2]; // Initialize {read, write} array for pipe
    if(pipe(p) == -1) {
        return returnError('p'); // Check for errors with pipe()
    }
    int Child1, Child2; // Initialize both children
    if((Child1 = fork()) == -1){ // Perform first fork and check for error
        return returnError('f');
    }
    else if(Child1 == 0) { // Actions for first child
        close(1); // Close stdout
        close(p[0]); // Close read end of pipe
        dup2(p[1], 1); // Write to pipe

        execlp("ls", "ls", "-F", NULL); // Execute 'ls -F'
    }
    else { // Actions for parent after first fork
        wait(NULL); // Wait for the first child to terminate
        if((Child2 = fork()) == -1){ // Perform second fork and check for error
            return returnError('f');     
        }
        else if(Child2 == 0) { // Actions for second child
            close(0); // Close stdin
            close(p[1]); // Close write end of pipe
            dup2(p[0], 0); // Read from pipe
            close(p[0]); // Close read end of pipe

            execlp("nl", "nl", NULL); // Execute 'nl'
        }
        else{
            close(p[1]); // Close write end of pipe
            wait(NULL); // Wait for the second child to terminate
            close(p[0]); // Close read end of pipe
        }
    }
    return 0;
}
int returnError(char c){ // Function that prints error type and exits
    if(c == 'f')
        perror("Fork");
    else if(c == 'p')
        perror("Pipe");
    return 1;
}