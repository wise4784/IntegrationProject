
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

const int *bl_fd;
char *bl_dev0 = "/dev/ttyUSB0";
char *bl_dev1 = "/dev/ttyUSB1";
pthread_mutex_t bl_mutx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  bl_cond = PTHREAD_COND_INITIALIZER;

char read_bl_ins =0;

void init_bl(void)
{
    int *tmp_fd =(int *)malloc(sizeof(int));
    int bl_mutx_state;
    *tmp_fd = serial_config(bl_dev0);
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
    char bl_rc_data =0;
    char bl_ins =0;
    recv_data(*bl_fd);
    if((bl_rc_data != 'a') || (bl_rc_data != 'b') || (bl_rc_data != 'c') || (bl_rc_data != 'd'))
    {
        if(bl_rc_data =='a')
            bl_ins =1;
        else if(bl_rc_data =='b')
            bl_ins =2;
        else if(bl_rc_data =='c')
            bl_ins =3;
        else if(bl_rc_data =='d')
            bl_ins =4;
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
}
