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
#define SEMA "7654"

typedef struct search{
	int call[2];
	int cursor;
	char keyword[MAX];
}search;

void init_search(search* search){
	search->call[0] = 0;
	search->call[1] = 0;
	search->cursor = 0;
 	*search->keyword = '\0';
}


