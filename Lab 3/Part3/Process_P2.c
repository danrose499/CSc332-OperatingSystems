/*
Task: Copy the contents of source.txt into destination1.txt and destination2.txt as per the following procedure until you reach the end of the file:
    1. Read the next 50 characters from source.txt, and write to destination1.txt
    2. Then the next 100 characters are read from source.txt and written into destination2.txt.
destination1.txt and destination2.txt were created in Process_P1.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 150

int closeAll(int file1, int file2, int file3);

int main() {
	int source, rd, acs, dest1, dest2;
    //Open source and destination files
    source = open("source.txt", O_RDONLY); 

    /* When opening the destination files, O_CREATE
       isn't needed, as Parent_Process will ensure
       that Process_P1 executes before Process_p2.
       Therefore, the files must already exist unless
       there was an error while executing Process_P1. */

    dest1 = open("destination1.txt", O_WRONLY, S_IRWXU); 
    dest2 = open("destination2.txt", O_WRONLY, S_IRWXU);

    //Check if there was an error opening
    if (source < 0 || dest1 < 0 || dest2 <0) {
        perror("open");
        closeAll(source, dest1, dest2);
        return 1;
    }  
	
    //Read source file and write to destination file
    int lengthRead, lengthWrite, writeA, writeB;
    char buffer[BUFSIZE];

    while( (lengthRead = read(source, buffer, BUFSIZE)) > 0 ) { //Read 150 chars of source file
        //Set the amount of chars needed to write to destination 1. The line below ensures that, at most, only the first 50 chars will be written
        lengthWrite = (lengthRead < 50) ? lengthRead : 50; 
        writeA = write(dest1, buffer, lengthWrite); //Write next 50 chars from buffer to destination 1 file
        if (lengthWrite < 50) // If less than 50 characters were read, exit the loop as there's nothing to write to destination 2.
            break;
        writeB = write(dest2, buffer+50, lengthRead-50); //Write next 100 chars from buffer to destination 2 file

        if (writeA == -1 || writeB == -1) { //Check for errors writting
            perror("write");
            closeAll(source, dest1, dest2);
            return 1;
        }
    }
    return closeAll(source, dest1, dest2);
}

int closeAll(int file1, int file2, int file3) {
    if (close(file1) != 0 || close(file2) != 0 || close(file3) != 0) { //Check for errors closing
        perror("close");
        return 1;
    }
    else
        return 0;
}
