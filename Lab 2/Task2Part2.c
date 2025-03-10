/*
Task: Consider the parent process as P. 
The program consists of fork() system call statements placed at different points in the code to create new processes Q and R.
The program also shows three variables: a, b, and pid - with the print out of these variables occurring from various processes. 
Show the values of pid, a, and b printed by the processes P, Q, and R.
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int a=10, b = 25, fq=0, fr=0;
    fq = fork(); // fork a child - call it Process Q
    if(fq==0) { // Process Q (before fork)
        a=a+b; 
        //print values of a, b, and process_id
        printf("\nProcess Q (before fork)\na: %d. b: %d. process_id: %d\n", a, b, getpid());

        fr = fork(); // fork another child - call it Process R
        if(fr!=0){ // Prcoess Q (after fork)
            b=b+15;
            //print values of a, b, and process_id
            printf("\nProcess Q (after fork)\na: %d. b: %d. process_id: %d\n", a, b, getpid());

        } else { // Process R
            a=(a*b)+20;
            //print values of a, b, and process_id
            printf("\nProcess R\na: %d. b: %d. process_id: %d\n", a, b, getpid());

        }
    } else { // Process P
        b=a+b-5;
        //print values of a, b, and process_id
        printf("\nProcess P\na: %d. b: %d. process_id: %d\n", a, b, getpid());

    }
    return 0;
}
