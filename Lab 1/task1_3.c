/*
3. Repeat part 2 (by writing a new C program) as per the following procedure:
(a) Read the next 50 characters from source.txt, and among characters read, replace each char- acter ’5’ with character ‘A’’ 
All characters are then written in destination.txt
(b) Write characters "XYZ" into file destination.txt
(c) Repeat the previous steps until the end of file source.txt. The last read step may not have 50
characters.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 50

int main (int argc, char* argv[]) {

    // Open source and destinatino files
    int source = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_WRONLY | O_CREAT);

    //Check if there was an error opening
    if (source < 0 || dest < 0) {
        perror("open");
        close(source); 
        close(dest);
        return 1;
    }     

    //Read source file and write to destination file
    int length, writeA, writeB;
    char buffer[BUFSIZE];

    while( (length = read(source, buffer, BUFSIZE)) > 0 ) { //Read 50 chars of source file
        for(int i = 0; i < length; i++) { //Iterate through buffer and replace 5s with As
            if(buffer[i] == '5') {
                buffer[i] = 'A';
            }
        }

        writeA = write(dest, buffer, length); //Write buffer to destination file
        writeB = write(dest, "XYZ", 3); //Write "XYZ" afterwords

        if (writeA == -1 || writeB == -1) //Check for errors writting
            perror("write");

    }
    if(length == -1) //Check for errors reading
        perror("read");

    //close
    if (close(source) != 0 || close(dest) != 0) {
        perror("close");
        return 1;
    }

    return 0;
}
