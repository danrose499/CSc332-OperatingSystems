/*
Task: After the successful creation of executables for Process_P1 and Process_P2 do the following: 
Write a C program and call it Parent_Process.c. Execute the files as per the following procedure using execv system call:
    1. Fork a child process, say Child 1 and execute Process_P1. This will create two destination files according to Step 1.
    2. After Child 1 finishes its execution, fork another child process, say Child 2, and execute Process_P2 that accomplishes the procedure described in Step 2.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pid_1, pid_2;

	pid_1 = fork(); //Fork first child
	if (pid_1 == 0) { //First Child process
		char *const argv[] = {"Process_P1", NULL};
		execv(argv[0], argv);  // Execute Prcs_P1
		printf("EXECV Error!\n");
	}
	else { //Parent process
		wait(NULL); //Wait for first child to die
		pid_2 = fork(); //Fork second child

		if (pid_2 == 0) { //Second child process
			char *const argv[] = {"Process_P2", NULL};
			execv(argv[0], argv);  // Execute Prcs_P2
			printf("EXECV Error!\n");
		}
	}
	return 0;
}
