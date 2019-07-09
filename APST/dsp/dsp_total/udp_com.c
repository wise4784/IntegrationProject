#include "udp_com.h"
#include "dsp_total.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

pthread_mutex_t udp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t udp_mutex_cond = PTHREAD_COND_INITIALIZER;

char read_mcu_state =0;
int sock = -1;

void init_udp(sockad_in *server_addr, sockad_in *client_addr,uint16_t port)
{
    read_mcu_state = 0;
    pthread_mutex_init(&udp_mutex,NULL);
    pthread_cond_init(&udp_mutex_cond,NULL);
    sock = socket(PF_INET, SOCK_DGRAM,0);

    if(sock == -1)
    {
        printf("Socket creation failed\r\n");
        exit(1);
    }

    memset(server_addr,0,sizeof(*server_addr));
    server_addr->sin_family = AF_INET; //IPv4 Internet
    server_addr->sin_port = htons(port); //port_num 7777(host to network short(Big Endian))
    server_addr->sin_addr.s_addr = htonl(INADDR_ANY); //assign automatically an IP address

    if(bind(sock,(sockad *)server_addr,sizeof(*server_addr)) == -1)
    {
        printf("Bind execution error\r\n");
        exit(1);
    }

    usleep(100);
}

void Udp_Send2MCU(sockad_in *client_addr, int ins,int setcnt, int setdeg)
{
    char buff_snd[10] ={0,0,0,0,0,0,0,0,0,0};

    if(ins == 5)
    {
        buff_snd[0] = 'o';
        printf("laser off ins o\r\n");
    }
    else if(ins == 4)
    {
        buff_snd[0] = 'l';
        printf("laser ins l\r\n");
    }
    else if(ins == 3)
    {
        buff_snd[0] = 't';
        printf("shoot ins t\r\n");
    }
    else if(ins ==2)
    {
        buff_snd[0] = 'd';
        printf("degree ins d\r\n");
    }
    else if(ins ==1)
    {
        buff_snd[0] = 's';
        printf("standard ins s\r\n");
    }
    else
    {
        buff_snd[0] = 'g';
        printf("send_data g\r\n");
    }

    buff_snd[1] = (setcnt & 0xFF000000) >> 24;
    buff_snd[2] = (setcnt & 0x00FF0000) >> 16;
    buff_snd[3] = (setcnt & 0x0000FF00) >> 8;
    buff_snd[4] = setcnt & 0x000000FF;
    buff_snd[5] = (setdeg & 0xFF000000) >> 24;
    buff_snd[6] = (setdeg & 0x00FF0000) >> 16;
    buff_snd[7] = (setdeg & 0x0000FF00) >> 8;
    buff_snd[8] = setdeg & 0x000000FF;
    sendto(sock, buff_snd, 10, 0, (sockad *)client_addr, sizeof(*client_addr));
}

void Udp_Receive2MCU(sockad_in *client_addr)
{
    int client_addr_size = sizeof(*client_addr);
    char rcv_buf[3] = {0,0,0};
    volatile char mcu_state =0;

#if 1
    recvfrom(sock, rcv_buf, 3,MSG_DONTWAIT , (sockad *)client_addr, &client_addr_size);
#else
    recvfrom(sock, rcv_buf, 3, 0 , (sockad *)client_addr, &client_addr_size);
#endif

    if(rcv_buf[0] == 's')
    {
        	mcu_state = rcv_buf[1];
        	printf("Receive udp data %c%d\r\n", rcv_buf[0],rcv_buf[1]);
    }

    if(read_mcu_state == 0)
    {
        read_mcu_state = mcu_state;
    }
}

void close_udp(void)
{
    close(sock);
    pthread_mutex_destroy(&udp_mutex);
    pthread_cond_destroy(&udp_mutex_cond);
    printf("\r\n close udp\r\n");
}

