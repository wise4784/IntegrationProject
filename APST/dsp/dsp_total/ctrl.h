#ifndef CTRL_H
#define CTRL_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>

//p 2.63 i 8.4 d 0.0015;
#define ENCODER_REF (double)0.3515625 //(deg/bit) (360/1024)
#define MOTOR_REF (double)360.0 //(cnt/rpm)
#define MOTOR_COF 30.0
#define PC (double)0.980
#define IC (double)0.015
#define DC (double)0.005
#define PGAIN (double)(PC*ENCODER_REF*MOTOR_COF)
#define IGAIN (double)(IC*ENCODER_REF*MOTOR_COF)
#define DGAIN (double)(DC*ENCODER_REF*MOTOR_COF)

#define MAX_DEG 320
#define MAX_RPM 30
#define MAX_CNT MAX_RPM*MOTOR_REF

#define VELO    1.0
#define GRAVITY 9.8

extern pthread_mutex_t ctrl_mutex;
extern pthread_mutex_t ctrl_mutex2;

extern int cur_dg;
extern float trg_dg;
extern int distance;
extern int setCNT;
extern int degree;
extern int ct_flag;
extern int shoot_ready_flag;
extern int shoot_fin_flag;
extern int shoot_flag;
extern int laser_flag;

void init_ctrl(void);
void close_ctrl(void);
void set_ctrl_flag(void);
int usr_abs(int intdata);
int error_cal(int target_deg, int current_deg);
int position_pid(int target_deg, int current_deg,int reset);
void manual_mod(void);
void Auto_Deg_ctrl(void);
void Auto_mod(void);
void sys_cotroller(void);
#endif
