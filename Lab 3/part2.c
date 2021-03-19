/*
Task: Write a program where a child is created to execute a command that shows all files (including hidden files) in a directory with information such as permissions, owner, size, and when last modified. 
Use execvp(...). Announce the successful forking of child process by displaying its PID.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {	
	int pid = fork();
	if(pid == 0) { // Child case
		printf("Child PID: %d\n", getpid()); 
        	char *argv[] = {"ls", "-laxo", NULL};
		execvp(argv[0],argv);  
	}

	else { // Parent case
        	wait(NULL);
		printf("Parent PID: %d\n", getpid());   	
	}
	return 0;
}
