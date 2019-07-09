#ifndef VCP_CAN_H
#define VCP_CAN_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

extern int lidar_data;
extern int encoder_data;
extern pthread_mutex_t can_mutx;

void init_can(void);
void send_ins2fpga(void);
int Recieve_fpga_data(void);
void close_can(void);

#endif
