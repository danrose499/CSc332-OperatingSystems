#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include "sem.h"

#define CHILD 0 // Child case of fork when pid == 0
#define FAIL -1 // Fork fails when pid == -1

int paper = 0, tobacco = 0, match = 0;
int counter = 10; // Agent runs 10 times

int main() {
    int lock  =  semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    int agent = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    int smoker_tobacco = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    int smoker_match   = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    int smoker_paper   = semget(IPC_PRIVATE, 1, 0666|IPC_CREAT);
    sem_create(lock,           1);
    sem_create(agent,          0);
    sem_create(smoker_tobacco, 0);
    sem_create(smoker_match,   0);
    sem_create(smoker_paper,   0);

    srand(time(0)); // Seeding rand for agent

    int pid, status;

    if((pid = fork()) == FAIL) { 
        perror("fork");
		exit(1);
    }
    if(pid == CHILD) { // First child: Agent Process
        while(counter > 0) {
            P(lock);
            printf("\n\nAgent turn: %d:\n", counter);
            int randNum = (rand() % 3) + 1; // Pick a random number from 1-3
            if (randNum == 1) {
                tobacco++;
                paper++;
                printf("Agent gives tobacco and paper.\n");
                V(smoker_match); // Wake up smoker with match
            } else if (randNum == 2) {
                tobacco++;
                match++;
                printf("Agent gives tobacco and match.\n");
                V(smoker_paper); // Wake up smoker with paper
            } else {
                match++;
                paper++;
                printf("Agent gives match and paper.\n");
                V(smoker_tobacco); // Wake up smoker with tobacco
            }
            V(lock);
            P(agent); // Agent sleeps
            counter--;
        }
    }
    else { // Parent process forks again...
        if((pid = fork()) == FAIL) {
            perror("fork");
		    exit(1);
        }
        if(pid == CHILD) { // Second child: Tobacco smoker
            while(1) { // Tobacco
                P(smoker_tobacco); // Sleep right away
                P(lock);
                paper--;
                match--;
                printf("Tobacco Smoker took them to smoke.\n");
                V(agent);
                V(lock);
            }
        }
        else { // Parent process forks again...
            if((pid = fork()) == FAIL) {
                perror("fork");
                exit(1);
            }
            if(pid == CHILD) { // Third child: Paper smoker
                while(1){ // Paper
                    P(smoker_paper); // Sleep right away
                    P(lock);
                    tobacco--;
                    match--;
                    printf("Paper smoker took them to smoke.\n");
                    V(agent);
                    V(lock);
                }
            }
            else { // Parent process forks a final time...
                if((pid = fork()) == FAIL) {
                    perror("fork");
                    exit(1);
                }
                if(pid == CHILD) { // Fourth child: Match smoker
                    while(1){ // Match
                        P(smoker_match); // Sleep right away
                        P(lock);
                        tobacco--;
                        paper--;
                        printf("Match smoker took them to smoke.\n");
                        V(agent);
                        V(lock);
                    }
                }
                else {
                    // Parent process waits for the agent process to finish
                    pid = wait(&status);
                    printf("Process(pid = %d) exited with the status %d. \n", pid, status);
                    // Kill semaphores (need to fix errors...)
//                     semkill(lock); 
//                     semkill(agent);
//                     semkill(smoker_tobacco);
//                     semkill(smoker_match);
//                     semkill(smoker_paper);
                }
            }
        }
    }
    return 0;
}
