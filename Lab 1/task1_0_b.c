/*
0. (b)Write a C program where open system call creates a new file (say, destination.txt) and then opens it. 
(Hint: use the bitwise OR flag)
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char* argv[]) {
    int fd;
    //Opens file in read only mode and creates the file if it doesn't yet exist
    fd = open(argv[1], O_RDONLY|O_CREAT);
    //Checks for errors opening
    if(fd == -1) {
        perror("open");
        return 1; 
    }
    else {
        printf("\n Open() Successful\n");
        /* open() succeeded, now one can do read operations on the
        file opened since we opened it in read-only mode. Also once done
        with processing, the file needs to be closed.*/
    }
    //Checks for errors closing
    if (close(fd) != 0 ) {
        perror("close");
        return 1;
    }
    else {
        printf("\n Close() Successful\n");
    }
    return 0;
}
