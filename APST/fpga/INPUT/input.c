#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#define IN				0
#define OUT				1

#define GPIO_MAP_SIZE		0x10000
#define GPIO_DATA_OFFSET	0x00
#define GPIO_TRI_OFFSET		0x04

void usage(void)
{
	printf(" *argv[0] -d <UIO_DEV_FILE> -i|-o <VALUE>\n");
	printf("	-d					UIO device file. e.g. /dev/uio0");
	printf("	-i					INPUT from GPIO\n");
	printf("	-o <VALUE>			OUTPUT to GPIO\n");

	return;
}

int main(void)
{
	int fd;
	int direction = IN;
	char *uiod;
	int value = 0;
	void *ptr;

	printf("GPIO UIO TEST\n");	
	uiod = "/dev/uio0";

	fd = open(uiod, O_RDWR);
	if(fd < 1)
	{
		printf("Invalid UIO device file : %s.\n",uiod);
		usage();
	
		return -1;
	}
	
	ptr = mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
	
	while(1)
	{
		*((unsigned *)(ptr + GPIO_TRI_OFFSET)) = 1023;
		value = *((unsigned *) (ptr + GPIO_DATA_OFFSET));
		printf("led-app : input : %08x\n",value);
	}
	munmap(ptr,GPIO_MAP_SIZE);

    return 0;
}
