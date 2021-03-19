/*
Task: Write a special simple command interpreter that takes command and its arguments.
This interpreter is a program where the main process creates a child process to execute the command using exec() family functions.
After executing the command, it asks for a new command input (i.e., parent wait for child).
The interpreter program will get terminated when the user enters quit.
*/

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
