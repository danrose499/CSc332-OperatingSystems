#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define BUFSIZE 1024

void printcontent(char* argv[]) {

    //Open file
    int fd = open(argv[1], O_RDWR | O_CREAT), length;
    char buffer[BUFSIZE];

    if (fd < 0) {
        perror("open");
        return;
    }       

    //Read file and write to cout
    while( (length = read(fd, buffer, BUFSIZE)) > 0 ) { 
       write(1, buffer, length);
       printf("%d %s", length, buffer);
    }

    //close
    if ( close(fd) != 0)
        perror("close");
}

int main (int argc, char* argv[]) {
    printcontent(argv);
}
