#include "csapp.h"

unsigned int snooze(unsigned int secs) {
	unsigned int last_secs = sleep(secs);
	printf("Slept for %d of %d secs.\n", secs-last_secs, secs);
	return last_secs;
}

void sigint_handler(int sig) {
	return;
}

int main(int argc, char *argv[]) {

    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        unix_error("signal error");
    }

    snooze(atoi(argv[1]));
    return 0;
}