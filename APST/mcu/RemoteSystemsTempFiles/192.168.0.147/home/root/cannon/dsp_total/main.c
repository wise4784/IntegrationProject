#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include "udp_com.h"
#include "vcp_can.h"
#include "bluetooth.h"

#define NUM_OF_THREAD 3
#define BUFF_SIZE 32
#define UDP_PORT 7777

int setCNT = 72;
int degree = 35;

void *serial_thread(void *x);
void *can_thread(void *x);
void *udp_thread(void *x);


void *serial_thread(void *x)
{
    printf("init_bl_start\r\n");
    init_bl();
    printf("init_bl_success\r\n");

    while(1)
    {
        recieve_bl();

        if(read_bl_ins>0)
        {
            if(read_bl_ins==1)
                printf("left\n");
            else if(read_bl_ins==2)
                printf("up\n");
            else if(read_bl_ins==3)
                printf("right\n");
            else if(read_bl_ins==4)
                printf("down\n");
            else if(read_bl_ins==5)
                printf("laser\n");
            else if(read_bl_ins==6)
                printf("shoot\n");
            else if(read_bl_ins==7)
                printf("mode\n");
            else
                printf("start\n");
            read_bl_ins = 0;
        }
    }

    close_bl_dv();
}

void *can_thread(void *x)
{
    int can_success =0;
    printf("init_can_start\r\n");
    init_can();
    printf("init_can_success\r\n");

    while(1)
    {
        can_success = Recieve_fpga_data();
        if(can_success == -1)
            usleep(50);
        else
            usleep(10000);
    }

    close_can();
}

void *udp_thread(void *x)
{
    int sock = -1;
    int first_recieve =1;
    sockad_in   server_addr;
    sockad_in   client_addr;

    printf("init_udp_start\r\n");
    init_udp(&sock, &server_addr, &client_addr, UDP_PORT);
    printf("init_udp_success\r\n");
    printf("socket %d\r\n");

    while(1)
    {
        Udp_Receive2MCU(&sock, &client_addr);

        if(read_mcu_state)
        {
            if(((read_mcu_state & 0X04)>>2) && first_recieve)
            {
                Udp_Send2MCU(&sock, &client_addr,STR_MCU,0,0); //start mcu g instrument
                first_recieve =0;
            }

            Udp_Send2MCU(&sock, &client_addr,SET_MCU,setCNT,degree);
            read_mcu_state = 0;
        }
    }

    close(sock);
}

int main(void)
{
    pthread_t t_id[NUM_OF_THREAD];
    int thread_cnt =0;

    pthread_create(&t_id[0], NULL, udp_thread, NULL);
    pthread_create(&t_id[1], NULL, can_thread, NULL);
    pthread_create(&t_id[2], NULL, serial_thread, NULL);

    for(thread_cnt=0 ;thread_cnt<NUM_OF_THREAD; thread_cnt++)
        pthread_detach(t_id[thread_cnt]);

    while(1);
}
