/*
Task: Create two files namely, destination1.txt and destination2.txt with read, write and execute permissions.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    //Create 2 destination files (if they don't exist) with read/write/execute permissions
	int fd1 = open ("destination1.txt", O_CREAT | O_RDWR, S_IRWXU), fd2 = open("destination2.txt", O_CREAT | O_RDWR, S_IRWXU); 	
	if (fd1 == -1 || fd2 == -1) //Check for errors opening either file
		perror("open");
    //Close both files
	close(fd1);
    close(fd2);
    return 0;
}
