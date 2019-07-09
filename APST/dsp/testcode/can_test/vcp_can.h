#ifndef VCP_CAN_H
#define VCP_CAN_H

#include "can_serial.h"
#include "vcp_can.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

extern int lidar_data;
extern int encoder_data;

void init_can(void);
void send_ins2fpga(void);
int Recieve_fpga_data(void);
void close_can(void);

#endif
