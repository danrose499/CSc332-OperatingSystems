#include <string.h> ///// My code: Added String header file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"


#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */

///// My code:
FILE *fDad, *fC1, *fC2;                      // Files to store below variables for each process
int countDad = 0, countC1 = 0, countC2 = 0;  // Keeps track of T(P) for each process
int dadWait = 0, son1Wait = 0, son2Wait = 0; // Keeps track of when each process is waiting to enter CS
void UpdateTP(char* Process);                // Updates the T(P) of the process passed to the function
void SetWait (char* Process, int status);    // Sets the wait status of the passed process to the passed status int
/////

main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables

	///// My code: added semaphore
	int sem1 = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
	sem_create(sem1,1);
	/////
	
	//Initialize the file balance to be $100
	fp1 = fopen("balance","w");
	bal1 = 100;
	fprintf(fp1, "%d\n", bal1);
	fclose(fp1);
	
	//Initialize the number of attempts to be 20
	fp4 = fopen("attempt", "w");
	N_Att = 20;
	fprintf(fp4, "%d\n", N_Att);
	fclose(fp4);

	///// My code: Initialized variables in my files
	fDad = fopen("dad", "w");
	fprintf(fDad, "%d %d", countDad, dadWait);
	fclose(fDad);
	
	fC1 = fopen("son1", "w");
	fprintf(fC1, "%d %d", countC1, son1Wait);
	fclose(fC1);

	fC2 = fopen("son2", "w");
	fprintf(fC2, "%d %d", countC2, son2Wait);
	fclose(fC2);
	/////

	
	//Create child processes that will do the updates
		if ((pid = fork()) == -1) 
	{
		//fork failed!
		perror("fork");
		exit(1);
	}
	
	if (pid == CHILD){
	//First Child Process. Dear old dad tries to do some updates.
	
		N=5;
		for(i=1;i<=N; i++)
		{
			///// My code:
			SetWait("Dad", 1); // Sets dadWait 'bool' so other processes know Dad is waiting to enter CS
			P(sem1);           // Lock CS while Dad is using it
			UpdateTP("C1");    // Increment first child's counter if it is waiting to enter CS
			UpdateTP("C2");    // Increment second child's counter if it is waiting to enter CS
			/////
			
			printf("Dear old dad is trying to do update.\n");
			fp1 = fopen("balance", "r+");
			fscanf(fp1, "%d", &bal2);
			printf("Dear old dad reads balance = %d \n", bal2);
			
			//Dad has to think (0-14 sec) if his son is really worth it
			sleep(rand()%15);
			fseek(fp1,0L,0);
			bal2 += 60;
			printf("Dear old dad writes new balance = %d \n", bal2);
			fprintf(fp1, "%d \n", bal2);
			fclose(fp1);

			printf("Dear old dad is done doing update. \n");

			///// My code:
			SetWait("Dad", 0); // Reset dadWait 'bool' so other processes know Dad isn't waiting to enter CS
			V(sem1);           // Unlock CS for other processes to use
			/////

			sleep(rand()%5);	/* Go have coffee for 0-4 sec. */
		}
	}
	
	else
	{
		//Parent Process. Fork off another child process.
		if ((pid = fork()) == -1)
		{
			//Fork failed!
			perror("fork");
			exit(1);
		}
		if (pid == CHILD)
		{
			printf("First Son's Pid: %d\n",getpid());
			//Second child process. First poor son tries to do updates.
			flag = FALSE;
			while(flag == FALSE) 
			{
				///// My code:
				SetWait("C1", 1);
				P(sem1);
				UpdateTP("Dad");
				UpdateTP("C2");
				/////

				fp3 = fopen("attempt" , "r+");
				fscanf(fp3, "%d", &N_Att);
				if(N_Att == 0)
				{
					fclose(fp3);
					flag = TRUE;
				}
				else
				{
					printf("Poor SON_1 wants to withdraw money.\n");
					fp2 = fopen("balance", "r+");
					fscanf(fp2,"%d", &bal2);
					printf("Poor SON_1 reads balance. Available Balance: %d \n", bal2);
					if (bal2 == 0)
					{
						fclose(fp2);
						fclose(fp3);
					}
					else
					{
						sleep(rand()%5);
						fseek(fp2,0L, 0);
						bal2 -=20;
						printf("Poor SON_1 write new balance: %d \n", bal2);
						fprintf(fp2,"%d\n", bal2);
						fclose(fp2);
						printf("poor SON_1 done doing update.\n");
						fseek(fp3,0L, 0);
						N_Att -=1;
						fprintf(fp3, "%d\n", N_Att);
						fclose(fp3);
					}
				}
				///// My code
				SetWait("C1", 0);
				V(sem1);
				/////
			}
		}
		else
		{
		//Parent Process. Fork off one more child process.
			if ((pid = fork()) == -1) 
			{
				//fork failed!
				perror("fork");
				exit(1);
			}
			if (pid == CHILD)
			{
				printf("Second Son's Pid: %d\n",getpid());
				//Third child process. Second poor son tries to do updates.
				flag1 = FALSE;
				while(flag1 == FALSE) 
				{
					///// My code:
					SetWait("C2", 1);
					P(sem1);
					UpdateTP("Dad");
					UpdateTP("C1");
					/////

					fp3 = fopen("attempt" , "r+");
					fscanf(fp3, "%d", &N_Att);
					if(N_Att == 0)
					{
						fclose(fp3);
						flag1 = TRUE;
					}
					else
					{
						printf("Poor SON_2 wants to withdraw money.\n");
						fp2 = fopen("balance", "r+");
						fscanf(fp2,"%d", &bal2);
						printf("Poor SON_2 reads balance. Available Balance: %d \n", bal2);
						if (bal2 == 0)
						{
							fclose(fp2);
							fclose(fp3);
						}
						else
						{
							sleep(rand()%5);
							fseek(fp2,0L, 0);
							bal2 -=20;
							printf("Poor SON_2 write new balance: %d \n", bal2);
							fprintf(fp2,"%d\n", bal2);
							fclose(fp2);

							printf("poor SON_2 done doing update.\n");
							fseek(fp3,0L, 0);
							N_Att -=1;
							fprintf(fp3, "%d\n", N_Att);
							fclose(fp3);
						}
					}
					///// My code:
					SetWait("C2", 0);
					V(sem1);
					/////
				}
			}
			else
			{
				//Now parent process waits for the child processes to finish
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			
				pid = wait(&status);
				printf("Process(pid = %d) exited with the status %d. \n", pid, status);
			}
			exit(0);
		}
		exit(0);
	}
	exit(0);
}

///// My code:
void UpdateTP(char* Process){                       // Updates the T(P) of the process passed to the function
	if (strcmp(Process, "Dad") == 0){
		fDad = fopen("dad", "r+");
		fscanf(fDad, "%d %d", &countDad, &dadWait); // Get current count and wait status for Dad process
		fseek(fDad, 0L, 0);
		if(dadWait == 1){                           // If dad is waiting to enter CS...
			countDad++;                             // ... increment its counter
		}
		fprintf(fDad, "%d %d", countDad, dadWait);  // Update counter
		fclose(fDad);
	}
	else if(strcmp(Process, "C1") == 0){
		fC1 = fopen("son1", "r+");
		fscanf(fC1, "%d %d", &countC1, &son1Wait);
		fseek(fC1, 0L, 0);
		if(son1Wait == 1){
			countC1++;
		}
		fprintf(fC1, "%d %d", countC1, son1Wait);
		fclose(fC1);
	}
	else if (strcmp(Process, "C2") == 0){
		fC2 = fopen("son2", "r+"); 
		fscanf(fC2, "%d %d", &countC2, &son2Wait);
		fseek(fC2, 0L, 0);
		if(son2Wait == 1){
			countC2++;
		}
		fprintf(fC2, "%d %d", countC2, son2Wait);
		fclose(fC2);
	}
}
void SetWait(char* Process, int status){            // Sets the wait status of the passed process to the passed status int
	if (strcmp(Process, "Dad") == 0){
		fDad = fopen("dad", "r+");
		fscanf(fDad, "%d %d", &countDad, &dadWait);
		fseek(fDad, 0L, 0);
		fprintf(fDad, "%d %d", countDad, status);  // Update dadWait status based on passed value
		fclose(fDad);
	}
	else if(strcmp(Process, "C1") == 0){
		fC1 = fopen("son1", "r+");
		fscanf(fC1, "%d %d", &countC1, &son1Wait);
		fseek(fC1, 0L, 0);
		fprintf(fC1, "%d %d", countC1, status);
		fclose(fC1);
	}
	else if (strcmp(Process, "C2") == 0){
		fC2 = fopen("son2", "r+"); 
		fscanf(fC2, "%d %d", &countC2, &son2Wait);
		fseek(fC2, 0L, 0);
		fprintf(fC2, "%d %d", countC2, status);
		fclose(fC2);
	}
}
/////
