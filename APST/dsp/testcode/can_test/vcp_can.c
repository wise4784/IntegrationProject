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
char *can_dev0 = "/dev/ttyUSB0";
char *can_dev1 = "/dev/ttyUSB1";
const int *can_fd;
pthread_mutex_t can_mutx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  can_cond = PTHREAD_COND_INITIALIZER;

void init_can(void)
{
	int *tmp_fd =(int *)malloc(sizeof(int));
	int can_mutx_state;
	*tmp_fd = serial_config(can_dev0);
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
	char can_tbuf[23] = {'x','1',' ',' ','0',' ','0',' ','0',' ','0',' ','0',' ','0',' ','0',' ','4','6','\r','\n',0};
	send_data(*can_fd,can_tbuf,22,0);
}

void Recieve_fpga_data(void)
{
    static unsigned int cnt =0;
    volatile unsigned int error_cnt = 0;
    int can_rv_buf_cnt =0;

    send_ins2fpga();

#if 1
    memset(data_buf,0,8);
    printf("receive\r\n");
    while((data_buf[0]==0) && (data_buf[2]==0))
    {
        recv_data(*can_fd);
    }
    for(can_rv_buf_cnt =0; can_rv_buf_cnt<8 ; can_rv_buf_cnt++)
    {
        printf("%d",data_buf[can_rv_buf_cnt]);
    }
#endif
}

void close_can(void)
{
	printf("close_can");
	close_dev(*can_fd);
	free((void *)can_fd);
	pthread_mutex_destroy(&can_mutx);
}
