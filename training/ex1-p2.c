#include <stdio.h>      // printf()
#include <unistd.h>     // sleep()
#include <sys/ipc.h>
#include <sys/shm.h>

#define  KEY_NUM     7654
#define  MEM_SIZE    1024

int main( void)
{
	int   shm_id;
	void *shm_addr;
	int i;
	struct shmid_ds;

	
	if ( -1 == ( shm_id = shmget( (key_t)KEY_NUM, MEM_SIZE, IPC_CREAT|0666)))
	{
		printf( "공유 메모리 생성 실패\n");
		return -1;
	}

	if ( ( void *)-1 == ( shm_addr = shmat( shm_id, ( void *)0, 0)))
	{
		printf( "공유 메모리 첨부 실패\n");
		return -1;
	}

	for(i=0; i<20; i++)
	{
		printf("shm_id: %d\n",shm_id);
		printf( "%s\n", (char *)shm_addr);    // 공유 메모리를 화면에 출력
		sleep( 1);
	}

	if( -1 == shmdt (shm_addr)){
		printf("failed to detach shm_addr\n");
		return -1;
	}
	else {
		printf("success to datach");
	}
	
	return 0;
}
