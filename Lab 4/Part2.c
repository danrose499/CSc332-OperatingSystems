/*
Task: There are 10 students enrolled in a course. The course covers x number of chapters from a textbook (x > 1). 
In each chapter y number of homework(s) are assigned (y ≥ 1). The average grade for each homework in all the chapters needs to be found out.
To solve this, write a program that has the main process as Director process, which reads a file containing grades of all homework of all chapters and creates x number of Manager processes. 
Each Manager process will take care of solving a chapter. Each manager process will create y number of Worker process and pass one homework to each of them and they calculate and print the average.
The input file should contain the data according to the value of x and y. 
[See "quiz_grades.txt" in this repository for the input file used.]

The Director process is responsible for opening and closing the input text file.
It stores the values in a two dimensional integer array with 10 rows. You may need to use the following C functions (in addition to the necessary file & process management system calls): 
      fopen(), fscanf(), fseek(), fclose().
      
The process for creating workers is analogous. 
The Manager parent either gives the worker child the path of the file to open and an offset at which to read its values, or else passes the actual values to be averaged as a list of parameters.
*/

#include <stdio.h>
#include <fcntl.h>      
#include <sys/types.h>   
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // Open input file with quiz grades and output file to print averages
    FILE* inputFile  = fopen("quiz_grades.txt","r"); 
    FILE* outputFile = fopen("quiz_averages.txt", "w");

    if (inputFile == NULL) { // Check for errors opening input file
        printf("File could not be found.\n"); 
        return 1; // Program terminates without any input
    }
    if (outputFile == NULL) // Check for errors opening output file
        printf("Error: Output file could not be found to write in!\n");
    

    // Hard coded values based on input file
    int chapters = 2, HWs = 2, students = 10;
    int columns = chapters * HWs;
    int grades[students][columns];

    for(int i = 0; i < students; i ++) {
        for(int j = 0; j < columns; j ++) {
            fscanf(inputFile, "%d", &grades[i][j]); // Save file contents in grades array
            printf("%d ", grades[i][j]); // Print file contents to console
            if((j + 1) % (columns) == 0) printf("\n");
        }
    }
    fclose(inputFile); // Done reading input file

    int n = chapters; // number of managers
    for(int i = 0; i < chapters; i++) { // loop to create managers equal to the number of chapters
        int manager = fork();
        if(manager < 0) { // check for error forking
            perror("fork");
            return 1;
        } else if (manager == 0) { // manager processes
            int h = HWs; // number of workers per manager
            for(int j = 0; j < HWs; j ++) { // each manager creating workers equal to the number of hw for that chapter
                int worker = fork();
                if (worker < 0) { // check for error forking
                    perror("fork");
                    return 1;
                } else if (worker == 0) { // worker processes
                    double sum = 0;
                    for(int s = 0; s < students; s ++) // Iterate down the column to get each students grade
                        sum += grades[s][(i*chapters) + j];
                    double avg = sum / students;
                    printf("Chapter: %d, Homework: %d, Average = %g / %d = %g\n", i + 1, j + 1, sum, students, avg); // Write to console
                    if(outputFile != NULL){ // Print to output file it it was able to be opened
                        fprintf(outputFile, "Chapter: %d, Homework: %d, Average = %g / %d = %g\n", i + 1, j + 1, sum, students, avg); // Write to output file
                    }
                    exit(0);
                }
            }
            while(h > 0) { // managers wait for the children
                wait(NULL);
                h--;
            }
            exit(0);
        }
    }
    while(n > 0) { // parent waiting for the managers
        wait(NULL);
        n--;
    }

    fclose(outputFile); // Close output file after writting
    return 0;
}
