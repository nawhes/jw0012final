#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "TSTree.h"
#include "nawhes.h"
#include <signal.h>

TSTree *root = NULL;
TSTree *found;

void *thread_push(void *buffer);
void *thread_print(void *buffer);
void sigInt(int signo);

int main(void){
	struct sigaction intsig;
	intsig.sa_handler = sigInt;
	intsig.sa_flags = 0;
	if(-1 == sigaction(SIGINT, &intsig, 0)){
		perror("failed sigaction\n");
		return -1;
	}

	char buffer[MAX];
	pthread_t threads[2];

	int rc;
	int status;
	
	char *result;

	int shm_id;
	search *shm_addr;
	sem_t *sem;
	sem = sem_open(SEMA, O_CREAT, 0666, 0);

	if(-1 == (shm_id = shmget((key_t)KEY_NUM, sizeof(search), IPC_CREAT|0666))){
		perror("failed shmget");
		return -1;
	}
	
	if((void *)-1 == (shm_addr = (search *)shmat(shm_id, NULL, 0))){
		perror("failed shmat");
		return -1;
	}
	while(1){
		sem_wait(sem);
	//	system("clear");
		if(shm_addr->keyword[shm_addr->cursor] != '\0'){
			if((shm_addr->call[0] != 1) && (shm_addr->call[1] != 1)){
				found = traverseTSTree(found, shm_addr->keyword + shm_addr->cursor);
				printf("keyword : %s\n", shm_addr->keyword);
				pthread_create(&threads[0], NULL, thread_print, (void *)buffer);
				shm_addr->cursor = shm_addr->cursor + 1;
			}
			else if(shm_addr->call[0]){
				shm_addr->keyword[shm_addr->cursor] = '\0';
				if(checkLastOne(found)){
					result = auto_complete(found, buffer, 0);
					printf("result : %s, %d\n",result, (int)strlen(result));
					strcat(shm_addr->keyword, result);
					shm_addr->cursor = shm_addr->cursor + strlen(result);
					found = traverseTSTree(found, result);
				}
				shm_addr->call[0] = 0;
			}
			else if(shm_addr->call[1]){
				shm_addr->keyword[shm_addr->cursor] = '\0';
				strcpy(buffer,shm_addr->keyword);
				pthread_create(&threads[1], NULL, thread_push, (void *)buffer);
				init_search(shm_addr);
				found = root;
				usleep(200);
			}
		//	printf("keyword: %s\n",shm_addr->keyword);
		}
		sem_post(sem);
		usleep(200);
	}
	return 0;
}

void *thread_push(void *buffer){
	insert(&root, (char *)buffer);
	printf("complete push\n");
	pthread_exit((void *) 0);
}

void *thread_print(void *buffer){
	printTSTree(found, (char *)buffer, 0);
	pthread_exit((void *) 0);
}

void sigInt(int signo){
	sigset_t sigset, oldset;
	sigfillset(&sigset);
	if(sigprocmask(SIG_BLOCK, &sigset, &oldset) < 0){
		printf("sigprocmask error\n");
	}
	freeTSTree(root);
	printf("root is free\n");
	exit(1);
}

