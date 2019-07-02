#ifndef UDP_COM_H
#define UDP_COM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define STR_MCU 0 //start communication with MCU
#define SET_MCU 1 //Set the Number to compare and degree

extern char read_mcu_state;
typedef struct sockaddr_in sockad_in;
typedef struct sockaddr sockad;

void init_udp(int *sock, sockad_in *server_addr, sockad_in *client_addr,uint16_t port);
void Udp_Send2MCU(int *sock, sockad_in *client_addr,int ins,int setCnt, int setDeg);
void Udp_Receive2MCU(int *sock, sockad_in *client_addr);
#endif
