#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFF_SIZE 1024

typedef struct {
	long data_type;
	int data_num;
	char data_buff[BUFF_SIZE];
} t_data;

int main(void) {
	int msqid;
	t_data data;

	//create the msg queue with key 1234
	//if the msg queue exists, ther just attaches
	if( -1 == (msqid = msgget( (key_t)7654, IPC_CREAT | 0600))) {
		perror("msgget() failed");
		exit(1);
	}

	while(1) {
		// lookup 1 among data_type
		if( -1 == msgrcv(msqid, &data, sizeof(t_data) - sizeof(long), 1, 0)) {
			perror("msgrev() failed");
			exit(1);
		}
		printf("%d - %s\n", data.data_num, data.data_buff);
	}
	return(0);
}
