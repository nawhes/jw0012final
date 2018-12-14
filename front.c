#include <sys/shm.h>
#include <signal.h>
//#include <curses.h>
#include "nawhes.h"

void sigInt(int signo);


int main(void){
	int shm_id, i, ch;
	search *shm_addr;
	struct sigaction intsig;
	intsig.sa_handler = sigInt;
	intsig.sa_flags = 0;

	sem_t *sem;
	sem = sem_open("sem", O_CREAT, 0666, 0);

	if(sigaction(SIGINT, &intsig, 0) == -1){
		printf("signal(SIGINT) error\n");
		return -1;
	}

	if(-1==(shm_id=shmget((key_t)KEY_NUM, sizeof(search), IPC_CREAT|0666))){
		printf("failed get\n");
		return -1;
	}

	if((void*)-1 == (shm_addr = (search *)shmat(shm_id, NULL, 0))){
		printf("failed attach\n");
		return -1;
	}

	for(shm_addr->cursor=0; (shm_addr->cursor<MAX) && ((ch=getchar()) != '0') && (ch != EOF); shm_addr->cursor++){
		getchar();
		if(ch != '8'){

		shm_addr->keyword[shm_addr->cursor + 1]='\0';
		shm_addr->keyword[shm_addr->cursor] = ch;
		printf("\n%d keyword: %s\n",shm_addr->cursor ,shm_addr->keyword);
		}
		else if (ch == '8'){
			shm_addr->call = 1;
		}

		sem_post(sem);
		usleep(200);
		sem_wait(sem);
		
	}
	
	return 0;
}

void sigInt(int signo){
	sigset_t sigset, oldset;
	sigfillset(&sigset);
	if(sigprocmask(SIG_BLOCK, &sigset, &oldset) < 0){
		printf("sigprocmask error\n");
	}
	printf("sigint\n");
}
