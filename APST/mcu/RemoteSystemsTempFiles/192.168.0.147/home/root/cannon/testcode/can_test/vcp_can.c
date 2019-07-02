#include "can_serial.h"
#include "vcp_can.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

extern char can_tx_buf[22];
extern char data_buf[9];
char dat_buf[22];
extern char *dev0;
const int *can_fd;
pthread_mutex_t can_mutx;

void init_can(void)
{
	int *tmp_fd =(int *)malloc(sizeof(int));
	int can_mutx_state;
	*tmp_fd = serial_config(dev0);
	can_fd = tmp_fd;

	can_mutx_state = pthread_mutex_init(&can_mutx,NULL);

	if(can_mutx_state !=0)
	{
	    printf("pthread_mutex error");
	}

	printf("access can");
}

void send_ins2fpga(void)
{
	char buf[23] = {'x','1',' ',' ','0',' ','0',' ','0',' ','0',' ','0',' ','0',' ','0',' ','4','6','\r','\n',0};
	write(*can_fd,buf,22);
	printf("send %s\n",buf);
}

void Recieve_fpga_data(void)
{
    static unsigned int cnt =0;
    volatile unsigned int error_cnt = 0;
    int can_rv_buf_cnt =0;
    send_ins2fpga();
#if 1
    memset(dat_buf,0,sizeof(dat_buf));
    printf("receive\r\n");

    pthread_mutex_lock(&can_mutx);
    read(*can_fd,dat_buf,21);
    pthread_mutex_unlock(&can_mutx);

    printf("error %d\n", error_cnt);

    for(can_rv_buf_cnt =0; can_rv_buf_cnt<21 ; can_rv_buf_cnt++)
    {
        printf("%c",dat_buf[can_rv_buf_cnt]);
    }
    printf("%d\n\n",cnt);
    cnt++;
#endif
}

void close_can(void)
{
	printf("close_can");
	close_dev(*can_fd);
	free((void *)can_fd);
	pthread_mutex_destroy(&can_mutx);
}
