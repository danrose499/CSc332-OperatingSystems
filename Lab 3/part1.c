/*
Task: Write a program where a child is created to execute command that tells you the date and time in Unix using execl(...). 
Note: you need to specify the full path of the file name that gives you date and time information. 
Announce the successful forking of child process by displaying its PID.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {	
	int pid = fork();
	if(pid == 0) { // Child case
		printf("Child PID: %d\n", getpid());  
        execl("/bin/date", "date", NULL);
	}
	else { // Parent case
        wait(NULL);
        printf("Parent PID: %d\n", getpid());
	}
	return 0;
}
