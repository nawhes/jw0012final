#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "TSTree.h"
#include "nawhes.h"

TSTree *root = NULL;

void *thread_push(void *word);

int main(void){
	int ho;
	char buffer[MAX];
	pthread_t threads;

	int rc;
	int status;
	
	char *result;

	int shm_id;
	search *shm_addr;
	sem_t *sem;
	sem = sem_open("sem", O_CREAT, 0666, 0);

	TSTree *found=NULL;
	
	if(-1 == (shm_id = shmget((key_t)KEY_NUM, sizeof(search), IPC_CREAT|0666))){
		printf("failed get");
		return -1;
	}
	
	if((void *)-1 == (shm_addr = (search *)shmat(shm_id, NULL, 0))){
		printf("failed attach");
		return -1;
	}
	found = root;
	while(1){
		sem_wait(sem);
		system("clear");
		if(shm_addr->call){
			if(checkLastOne(found)){
				result = auto_complete(found, buffer, 0);
				printf("result : %s\n",result);
				sleep(1);
				strcat(shm_addr->keyword, result);
				shm_addr->cursor = shm_addr->cursor + strlen(result) - 1;
			//	shm_addr->keyword[shm_addr->cursor] = '\0';
			}
			shm_addr->call = 0;
			found = traverseTSTree(root, shm_addr->keyword);
		}
		else {
			found = traverseTSTree(found, shm_addr->keyword + shm_addr->cursor);
			printf("keyword : %s\n", shm_addr->keyword);
			printTSTree(found, buffer, 0);
			if(0 == strncmp(shm_addr->keyword + shm_addr->cursor,"9",1)){
				printf("pushed\n");
				strcpy(buffer,shm_addr->keyword);
				pthread_create(&threads, NULL, thread_push, (void *)buffer);
				usleep(200);
				init_search(shm_addr);
				found = root;
			}
		}
		sem_post(sem);
		usleep(200);
	}
	return 0;
}

void *thread_push(void *word){
	insert(&root, (char *)word);
	printf("complete push\n");
	pthread_exit((void *) 0);
}	
