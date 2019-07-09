#include "can_serial.h"
#include "vcp_can.h"
#include "dsp_total.h"
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

int lidar_data = -1;
int encoder_data = -1;

pthread_mutex_t can_mutx = PTHREAD_MUTEX_INITIALIZER;

void init_can(void)
{
	int *tmp_fd =(int *)malloc(sizeof(int));
	int can_mutx_state;
	*tmp_fd = can_serial_config(can_dev0);
	can_fd = tmp_fd;

	can_recv_data(*can_fd);

	can_mutx_state = pthread_mutex_init(&can_mutx,NULL);

	if(can_mutx_state !=0)
	{
	    printf("pthread_mutex error\n");
	}

	printf("access can\n");
}

void send_ins2fpga(void)
{
#if 0
    char tx_dat[22] ="x1  0 0 0 0 0 0 0 f\r\n";
	write(*can_fd,can_tx_buf,21);
#else
	char can_tbuf[18] = {0x02,0x00,0x08,0x40,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x58,0x03};
//	char can_tbuf[18] = {0x02,0x00,0x08,0x40,0x00,0x00,0x00,0x01,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0x0f,0xC1,0x03};
//  char can_tbuf[18] = {0x02,0x00,0x08,0x40,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x46,0x8F,0x03};
//	char can_tbuf[18] = {0x02,0x80,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0x03};
//	char can_tbuf[18] = {'T',0};
	can_send_data(*can_fd,can_tbuf,18,0);
#endif

}

int Recieve_fpga_data(void)
{
    unsigned int can_txdata =0;
    int can_txdata_H =0;
    int can_txdata_L =0;
    send_ins2fpga();

#if 1
    memset(data_buf,0,8);
    printf("receive\r\n");
    can_recv_data(*can_fd);
    while(((long)*data_buf)==0)
    {
        can_recv_data(*can_fd);
    }

    if((data_buf[0]=='F') && (data_buf[5]=='E'))
    {
        can_txdata = *((unsigned int *)(data_buf+1));
        can_txdata_H = (can_txdata & (0xFFFF0000))>>16;
        can_txdata_L = can_txdata & (0x0000FFFF);

        if(lidar_data == -1)
            lidar_data = can_txdata_L;
        if(encoder_data == -1)
            encoder_data = can_txdata_H;
        return 0;
    }
    else
    {
        can_recv_data(*can_fd);
        usleep(5);
        can_recv_data(*can_fd);
    }
    return -1;
#else
    return 0;
#endif
}

void close_can(void)
{
	printf("close_can");
	can_close_dev(*can_fd);
	free((void *)can_fd);
	pthread_mutex_destroy(&can_mutx);
}
