#ifndef UDP_COM_H
#define UDP_COM_H

#include "can_serial.h"
#include "vcp_can.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void init_can(void);
void send_ins2fpga(void);
void Recieve_fpga_data(void);
void close_can(void);

#endif
