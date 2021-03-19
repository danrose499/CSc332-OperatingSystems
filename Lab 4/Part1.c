#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    char command[128];
    char* token;    

    printf("\n$ "); //gets ready for first command input
    scanf(" %[^\n]s", command); // collects first user input —- %[^\n]s allows for whitespaces

    while(strcmp(command, "quit")) { //Exits while loop when command == "quit"

        char* cmdArray[5] = {NULL}; // Initializes command array to NULL so execvp knows where to stop
        token = strtok(command, " "); // Breaks up input string after first space
        int i = 0; // Index counter for command array
        while(token != NULL) { 
            cmdArray[i++] = token; // Insert token into current index in array and increment index
            token = strtok(NULL, " "); // Seperate string at next space
        }
        int pid = fork();
        if (pid == 0) { // Child process
	  	    execvp (cmdArray[0], cmdArray);
	  	    printf ("EXECVP Failed\n");
        }
        wait(NULL); // Parent waits for child termination

        printf("\n$ "); //gets ready for next command input
        scanf(" %[^\n]s", command); // collects next user input
    }      
    printf("\nQuiting.\n");
    return 0;
}