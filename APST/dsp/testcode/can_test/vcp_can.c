#include "can_serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

extern char *dev0;
const int *can_fd;

void init_can(void)
{
	int *tmp_fd =(int *)malloc(sizeof(int));
	*tmp_fd = serial_config(dev0);
	can_fd = tmp_fd;
	printf("access can");
}

void send_ins2fpga(void)
{
	char buf = 'f';
	send_data(*can_fd,buf,1,1);
}

void close_can(void)
{
	printf("close_can");
	close_dev(*can_fd);
	free(can_fd);
}
