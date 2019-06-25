#include "udp_com.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

char read_mcu_state =0;

void init_udp(int *sock, sockad_in *server_addr, sockad_in *client_addr,uint16_t port)
{

    read_mcu_state = 0;
    *sock = socket(PF_INET, SOCK_DGRAM,0);

    if(*sock == -1)
    {
        printf("Socket creation failed\r\n");
        exit(1);
    }

    memset(server_addr,0,sizeof(*server_addr));
    server_addr->sin_family = AF_INET; //IPv4 Internet
    server_addr->sin_port = htons(port); //port_num 7777(host to network short(Big Endian))
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY); //assign automatically an IP address

    if(bind(*sock,(sockad *)server_addr,sizeof(*server_addr)) == -1)
    {
        printf("Bind execution error\r\n");
        exit(1);
    }

    usleep(100);
}

void Udp_Send2MCU(int *sock, sockad_in *client_addr, int ins,int setCnt, int setDeg)
{
    char buff_snd[10] ={0,0,0,0,0,0,0,0,0,0};

    if(ins)
    {
        buff_snd[0] = 's';
        buff_snd[1] = (setCnt & 0xFF000000) >> 24;
        buff_snd[2] = (setCnt & 0x00FF0000) >> 16;
        buff_snd[3] = (setCnt & 0x0000FF00) >> 8;
        buff_snd[4] = setCnt & 0x000000FF;
        buff_snd[5] = (setDeg & 0xFF000000) >> 24;
        buff_snd[6] = (setDeg & 0x00FF0000) >> 16;
        buff_snd[7] = (setDeg & 0x0000FF00) >> 8;
        buff_snd[8] = setDeg & 0x000000FF;
        sendto(*sock, buff_snd, 10, 0, (sockad *)client_addr, sizeof(*client_addr));
    }
    else
    {
        buff_snd[0] = 'g';
        printf("sand_data g\r\n");
        sendto(*sock, buff_snd, 2, 0, (sockad *)client_addr, sizeof(*client_addr));
    }

}

void Udp_Receive2MCU(int *sock, sockad_in *client_addr)
{
    int client_addr_size = sizeof(*client_addr);
    char rcv_buf[3] = {0,0,0};
    volatile char mcu_state =0;
#if 1
    recvfrom(*sock, rcv_buf, 3,MSG_DONTWAIT , (sockad *)client_addr, &client_addr_size);
#else
    recvfrom(*sock, rcv_buf, 3, 0 , (sockad *)client_addr, &client_addr_size);
#endif
    if(rcv_buf[0] == 's')
    {
        	mcu_state = rcv_buf[1];
        printf("Receive data %c%d\r\n", rcv_buf[0],rcv_buf[1]);
    }

    if(read_mcu_state == 0)
    {
        read_mcu_state = mcu_state;
    }
}

