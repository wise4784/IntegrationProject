#include "udp_com.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFF_SIZE 32
#define UDP_PORT 7777

int setCNT = 72;
int degree = 35;

int main(void)
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
            }

            Udp_Send2MCU(&sock, &client_addr,SET_MCU,setCNT,degree);
            read_mcu_state = 0;
        }
    }

    return 0;
}
