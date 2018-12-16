#include <sys/shm.h>
#include <signal.h>
//#include <curses.h>
#include "nawhes.h"

int main(void){
	int shm_id, i, ch;
	search *shm_addr;

	sem_t *sem;
	sem = sem_open(SEMA, O_CREAT, 0666, 0);

	if(-1==(shm_id=shmget((key_t)KEY_NUM, sizeof(search), IPC_CREAT|0666))){
		printf("failed get\n");
		return -1;
	}

	if((void*)-1 == (shm_addr = (search *)shmat(shm_id, NULL, 0))){
		printf("failed attach\n");
		return -1;
	}
	init_search(shm_addr);
	for(i = 0; (i + 1 < MAX) && ((ch=getchar()) != '0') && (ch != EOF); ){
		while(getchar() != '\n');
		shm_addr->keyword[shm_addr->cursor + 1] = '\0';
		shm_addr->keyword[shm_addr->cursor] = ch;
		i = shm_addr->cursor;
		if (ch == '`'){
			shm_addr->call[0] = 1;
		}
		if (ch == '\n'){
			shm_addr->call[1] = 1;
		}
		sem_post(sem);
		usleep(200);
		sem_wait(sem);
		printf("keyword: %s\n",shm_addr->keyword);

	}

	return 0;
}

