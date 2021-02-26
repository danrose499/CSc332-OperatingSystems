/*
2. The cp command copies the source file specified by the SourceFile parameter to the destination file specified by the DestinationFile parameter.
Write a C program that mimics the cp command using open() system call to open source.txt file in read-only mode
Copy the contents of it to destination.txt using read() and write() system calls.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main (int argc, char* argv[]) {

    //Open source file 
    int source = open(argv[1], O_RDONLY);
    int dest = open(argv[2], O_WRONLY | O_CREAT);
    int length;
    char buffer[BUFSIZE];

    //Check for errors opening
    if (source < 0 || dest < 0) {
        perror("open");
        close(source); 
        close(dest);
        return 1;
    }       

    //Read file and write to cout
    while( (length = read(source, buffer, BUFSIZE)) > 0 ) { 
       if(write(dest, buffer, length) == -1){
           perror("write");
       }
    }
    if(length == -1) //Check for errors reading
        perror("read");

    //close
    if (close(source) != 0 || close(dest) != 0){ //Check for errors closing
        perror("close");
        return 1;
    }
    return 0;
}
