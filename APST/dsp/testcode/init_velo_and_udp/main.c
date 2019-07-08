#include "udp_com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 32
#define UDP_PORT 7777

int setCNT = 72;
int setDeg = 35;

int Cnt_end_flag =  1;
int Deg_end_flag =  1;

void *can_thread (void *x)
{
    int sock = -1;
    int first_recieve =1;
    sockad_in   server_addr;
    sockad_in   client_addr;

    printf("init_start\r\n");
    init_udp(&sock, &server_addr, &client_addr, UDP_PORT);
    printf("init_success\r\n");
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
                read_mcu_state = 0;
            }
            else if(Cnt_end_flag && Deg_end_flag)
            {
                Udp_Send2MCU(&sock, &client_addr, SET_MCU, setCNT, setDeg);
                setCNT = 0;
                setDeg = 0;
                Cnt_Deg_end_flag =0;
                read_mcu_state = 0;
            }
        }
    }

    return NULL;
}

int main(void)
{
    pthread_t t_id1;
    pthread_create(&t_id1, NULL, can_thread, NULL);
    pthread_detach(t_id1);
    while(1)
    {

    }
}
