#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "sem.h"

#define CHILD      			0  			/* Return value of child proc from fork call */
#define TRUE       			0  
#define FALSE      			1

FILE *fp1, *fp2, *fp3, *fp4;			/* File Pointers */

main()
{
	int pid;						// Process ID after fork call
	int i;							// Loop index
	int N;							// Number of times dad does update
	int N_Att;						// Number of time sons allowed to do update
	int status;						// Exit status of child process
	int bal1, bal2;					// Balance read by processes
	int flag, flag1;				// End of loop variables

	/////
	bool dadWait = false, son1Wait = false, son2Wait = false; // Keeps track of when each process is waiting to enter CS
	int sem1 = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
	//int sem2 = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
	sem_create(sem1,1);
	//sem_create(sem2, 20);
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

	//////////
	FILE *fDad, *fC1, *fC2; 
	int countDad, countC1, countC2;
	fDad = fopen("dad", "w");
	countDad = 0;
	fprintf(fDad, "%d\n", countDad);
	fclose(fDad);
	
	fC1 = fopen("son1", "w");
	countC1 = 0;
	fprintf(fC1, "%d\n", countC1);
	fclose(fC1);

	fC2 = fopen("son2", "w");
	countC2 = 0;
	fprintf(fC2, "%d\n", countC2);
	fclose(fC2);
	//////////

	
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
			/////
			dadWait = true; // Sets dadWait bool so other processes know Dad is waiting to enter CS
			P(sem1); 
			if(son1Wait == true){ // If son 1 is waiting, increment its counter
				fC1 = fopen("son1", "r+"); 
				fscanf(fC1, "%d", &countC1);
				fseek(fC1, 0L, 0);
				countC1++;	      	
				fprintf(fC1, "%d\n", countC1);
				fclose(fC1);
			}
			if(son2Wait == true){ // If son 2 is waiting, increment its counter
				fC2 = fopen("son2", "r+"); 
				fscanf(fC2, "%d", &countC2);
				fseek(fC2, 0L, 0);
				countC2++;
				fprintf(fC2, "%d\n", countC2);
				fclose(fC2);
			}

			//P(sem2); 
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

			/////
			dadWait = false; // Dad finished process and is no longer waiting
			V(sem1);
			//V(sem2);
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
				///
				son1Wait = true;
				P(sem1);
				if(dadWait == true){
					fDad = fopen("dad", "r+");
					fscanf(fDad, "%d", &countDad);
					fseek(fDad, 0L , 0);
					countDad++;
					fprintf(fDad, "%d\n", countDad);
					fclose(fDad);
				}
				if(son2Wait == true){
					fC2 = fopen("son2", "r+");
					fscanf(fC2, "%d", &countC2);
					fseek(fC2, 0L, 0);
					countC2++;
					fprintf(fC2, "%d\n", countC2);
					fclose(fC2);
				}

				//P(sem2);
				///

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
				////
				son1Wait = false;
				V(sem1);
				//V(sem2);
				////
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
					/////
					son2Wait = true;
					P(sem1);
					
					if(dadWait == true){
						fDad = fopen("dad", "r+");
						fscanf(fDad, "%d", &countDad);
						fseek(fDad, 0L, 0);
						countDad++;
						fprintf(fDad, "%d\n", countDad);
						fclose(fDad);
					}
					if(son1Wait == true){
						fC1 = fopen("son1", "r+");
						fscanf(fC1, "%d", &countC1);
						fseek(fC1, 0L, 0);
						countC1++;
						fprintf(fC1, "%d\n", countC1);
						fclose(fC1);
					}

					//P(sem2);
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
					son2Wait = false;
					V(sem1);
					//V(sem2);
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
