#include "bluetooth.h"
#include "serial.h"
#include "dsp_total.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

extern char rx_buf[2];
const int *bl_fd;
char *bl_dev0 = "/dev/ttyUSB0";
char *bl_dev1 = "/dev/ttyUSB1";
pthread_mutex_t bl_mutx = PTHREAD_MUTEX_INITIALIZER;

char read_bl_ins =0;

void init_bl(void)
{
    int *tmp_fd =(int *)malloc(sizeof(int));
    int bl_mutx_state;
    *tmp_fd = serial_config(bl_dev1);
    bl_fd = tmp_fd;

    bl_mutx_state = pthread_mutex_init(&bl_mutx,NULL);

    if(bl_mutx_state !=0)
    {
        printf("pthread_mutex error");
    }

    printf("access bl");
}

void recieve_bl(void)
{
    char bl_ins =0;

#if 0
    char *bl_rc_data = rx_buf;
    memset(bl_rc_data,0,2);
    recv_data(*bl_fd);
#else
    char bl_rc_data[2] = {0,0};
    read(*bl_fd,bl_rc_data,1);
#endif

    if((*bl_rc_data >='a') && (*bl_rc_data <='h'))
    {
        bl_ins = *bl_rc_data - 'a' + 1;
    }

    if(read_bl_ins ==0)
    {
        read_bl_ins = bl_ins;
    }
}
void close_bl_dv(void)
{
    close_dev(*bl_fd);
    free((void *)bl_fd);
    pthread_mutex_destroy(&bl_mutx);
    printf("close bl\r\n");
}
