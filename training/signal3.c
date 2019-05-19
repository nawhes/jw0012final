#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

void clock_tick(int signo) {
	printf("\r%ld", time(NULL));
	alarm(1);
}

void sig_int(int ingno) {
	printf("signalled init\n");
}

int main(void) {
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);
	printf("\e[2J\e[H");

	signal(SIGALRM, clock_tick);
	signal(SIGINT, sig_int);
	clock_tick(-1);
	for(;;){
		pause();
	}
	exit(EXIT_SUCCESS);
}
