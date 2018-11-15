#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> //for ipc
#include <sys/msg.h>

#define BUFF_SIZE 1024

//for structiong a message format
typedef struct {
	long data_type; //used for msg type
	int data_num; //user data #1
	char data_buff[BUFF_SIZE];
}t_data;

int main(void) {
	int msqid;
	int ndx = 0;
	t_data data;

	//create the msg queue with key 1234
	if(-1 == (msqid = msgget((key_t)7654, IPC_CREAT | 0600))) {
		perror("mesget() error");
		exit(1);
	}

	printf("sender start!\n");

	//write data into the msg queue
	while(1) {
		data.data_type = (ndx++ %3) + 1; //data_type is 1, 2, 3
		data.data_num = ndx;
		sprintf(data.data_buff, "type=%ld, ndx=%d, mq!", data.data_type, ndx);

		//actual sending into the msgqueue
		if(-1 == msgsnd(msqid, &data, sizeof(t_data) - sizeof(long), 0)) {
			//	if(-1 == msgsnd(msqid, &data, sizeof(t_data) - sizeof(long), IPC_NOWAIT)) {
			perror("msgsnd() error or msg queue is full");
			exit(1);
		}

		printf("sender: %d - %s\n", data.data_num, data.data_buff);

		sleep(1);
	}

	return(0);
}
