#ifndef UDP_COM_H
#define UDP_COM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define STR_INS 0 //start communication with MCU
#define STD_INS 1 //Set the Number to compare and degree
#define DEG_INS 2
#define SHT_INS 3
#define LASER_INS 4
#define LASER_OFF_INS 5

extern char read_mcu_state;
extern pthread_mutex_t udp_mutex;
extern pthread_cond_t udp_mutex_cond;
typedef struct sockaddr_in sockad_in;
typedef struct sockaddr sockad;

void init_udp(sockad_in *server_addr, sockad_in *client_addr,uint16_t port);
void Udp_Send2MCU(sockad_in *client_addr,int ins,int setcnt, int setdeg);
void Udp_Receive2MCU(sockad_in *client_addr);
void close_udp(void);
#endif
