#include<sys/stat.h>
#include<errno.h>

void _exit(int status){ while(1);}

int _write(int fd, char* ptr, int len){return -1;}

void *_sbrk(int incr){
	return(void *) -1;
}
int _close(int fd){return -1;}

int _read(int fd, char*ptr, int len){return -1;}

int _lseek(int fd, int ptr, int dir){return -1;}

