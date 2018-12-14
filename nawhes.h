#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>

#define KEY_NUM 7654
#define MAX 64

typedef struct search{
	int call;
	int cursor;
	char keyword[MAX];
}search;

void init_search(search* search){
	search->call = 0;
	search->cursor = -1;
 	*search->keyword = '\0';
}


