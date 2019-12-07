#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
unsigned int snooze(unsigned int secs) {
    unsigned int left = sleep(secs);
    printf("Slept for %d of %d secs.\n", left, secs);
    return left;
}

int main(int argc, char const *argv[])
{
	int fd;
	fd = open("1.txt", O_RDWR, 0);
	dup2(fd, STDOUT_FILENO);
	printf("%d\n", fd);
	close(fd);
    // snooze(5);
    // return 0;
}

