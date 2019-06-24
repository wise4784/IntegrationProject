#ifndef POS_PID_H
#define POS_PID_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

//p 2.63 i 8.4 d 0.0015;
#define ENCODER_REF 0.3515625 //(deg/bit) (360/1024)
#define PC 1.000
#define IC 0.000
#define DC 0.000
#define MOTOR_REF 360 //(cnt/rpm)
#define PGAIN PC*ENCODER_REF
#define IGAIN IC*ENCODER_REF
#define DGAIN DC*ENCODER_REF
#endif
