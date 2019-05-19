#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void sig_usr1(int signo) {
	printf("signalled SIGUSR!");
	exit(EXIT_SUCCESS);
}

int main(void) {
	int ppid;
	if(!fork()){
		ppid = getppid();
		kill(ppid, SIGUSR1);

		exit(0);
	}

	else {
		signal(SIGUSR1, sig_usr1);
		wait(NULL);
	}

}
