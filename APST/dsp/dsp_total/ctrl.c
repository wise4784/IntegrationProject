#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include "ctrl.h"
#include "udp_com.h"
#include "dsp_total.h"

pthread_mutex_t ctrl_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ctrl_mutex2 = PTHREAD_MUTEX_INITIALIZER;

int shoot_ready_flag = 0;
int shoot_fin_flag =0;
int Auto_mode_sl = 0;
int ctrl_start_flag = 0;
int left_flag =0;
int right_flag =0;
int up_flag =0;
int down_flag =0;
int shoot_flag =0;
int laser_flag =0;

int ct_flag =0;

int cur_dg =0;
float trg_dg = 0;
int distance = 0;
int setCNT = 0;
int degree = 0;

void init_ctrl(void)
{
    int ctrl_mutx_state;
    ctrl_mutx_state = pthread_mutex_init(&ctrl_mutex,NULL);
    ctrl_mutx_state = pthread_mutex_init(&ctrl_mutex2,NULL);

    cur_dg =0;
    trg_dg = 0;
    setCNT = 0;
    degree = 0;

    if(ctrl_mutx_state !=0)
    {
        printf("pthread_mutex error\n");
    }
}

void close_ctrl(void)
{
    cur_dg =0;
    trg_dg = 0;
    setCNT = 0;
    degree = 0;
    pthread_mutex_destroy(&ctrl_mutex);
    pthread_mutex_destroy(&ctrl_mutex2);
}

void set_ctrl_flag(void)
{
    pthread_mutex_lock(&ctrl_mutex);
    if(ct_flag>0)
    {
        if(ct_flag == 1)
            left_flag =1;
        else if(ct_flag == 2)
            up_flag =1;
        else if(ct_flag == 3)
            right_flag =1;
        else if(ct_flag == 4)
            down_flag =1;
        else if(ct_flag == 5)
            laser_flag =1;
        else if(ct_flag == 6)
            shoot_flag =1;
        else if(ct_flag == 7)
            Auto_mode_sl ^= 1;
        else
            ctrl_start_flag ^= 1;

        ct_flag=0;
    }
    pthread_mutex_unlock(&ctrl_mutex);
}

int usr_abs(int intdata)
{
    if(intdata<0)
        intdata = -intdata;

    return intdata;
}

int error_cal(int target_deg, int current_deg)
{
    int min;
    int error1;
    int error2;
    int error3;

    error1 = target_deg-current_deg;
    error2 = error1 + 1024;
    error3 = error1 - 1024;

    min = error1;
    if(usr_abs(error2)<usr_abs(min))
        min = error2;
    if(usr_abs(error3) <usr_abs(min))
        min = error3;

    return min;
}

int position_pid(int target_deg, int current_deg,int reset)
{

    static unsigned int pos_pid_cnt = 0;
    int error =0;
    int d_error;
    static int error_sum = 0;
    static int error_old = 0;
    static int ouput_offset =0;
    int output =0;

    if(reset == 1)
    {
        error   =   error_cal(target_deg,current_deg);
        error_sum +=    (error + error_old)/2;
        d_error = error - error_old;
        output =(int)(PGAIN*(double)error +IGAIN*(double)error_sum+DGAIN*(double)d_error) + ouput_offset;

        pos_pid_cnt++;
        if(pos_pid_cnt>100000)
        {
            error_sum =0;
            ouput_offset  = IGAIN*error_sum;
            pos_pid_cnt =0;
        }
        error_old = error;
    }
    else
    {
        error_sum =0;
        error_old = 0;
        ouput_offset =0;
    }

    if(output>MAX_CNT)
        output = MAX_CNT;
    else if(output<-MAX_CNT)
        output = -MAX_CNT;

    return output;
}

void manual_mod(void)
{
    int delta_ps_deg;
    float delta_cn_deg;

    if(ctrl_start_flag)
    {
        delta_ps_deg = 20;
        delta_cn_deg = 10;
    }
    else
    {
        delta_ps_deg = 2;
        delta_cn_deg = 2;
    }

    if(left_flag)
    {
        setCNT = position_pid(cur_dg+delta_ps_deg, cur_dg, 1);
        left_flag =0;
    }
    else if(right_flag)
    {
        setCNT = position_pid(cur_dg-delta_ps_deg, cur_dg, 1);
        right_flag =0;
    }
    else
    {
        setCNT =0;
    }

    if(up_flag)
    {
        degree += delta_cn_deg;
        if(degree>=MAX_DEG)
            degree =MAX_DEG;
        up_flag =0;
    }
    else if(down_flag)
    {
        degree -= delta_cn_deg;
        if(degree<0)
            degree = 0;
        down_flag =0;
    }

    if(shoot_fin_flag)
        shoot_fin_flag =0;
}

int lidar_det_flag =0;

void Auto_Deg_ctrl(void)
{
    pthread_mutex_lock(&ctrl_mutex2);
    if(shoot_flag)
    {
        if(lidar_det_flag ==0)
        {
            degree = DEG_CAL*distance;
            lidar_det_flag =1;

            if(degree>=MAX_DEG)
                degree =MAX_DEG;
        }
    }
    else if(laser_flag)
    {
        if(lidar_det_flag ==0)
        {
            if((distance>=1500) && (distance<200))
            {
                degree += 2;
                if(degree>=MAX_DEG)
                    degree =MAX_DEG;
            }
            else
                lidar_det_flag =1;
        }
        else
        {
            degree =0;
            lidar_det_flag =0;
        }
    }

    if(shoot_fin_flag ==1)
    {
        degree =0;
        lidar_det_flag=0;
        shoot_flag =0;
        shoot_fin_flag =0;
    }
    pthread_mutex_unlock(&ctrl_mutex2);
}

void Auto_mod(void)
{
    if(ctrl_start_flag)
    {
        setCNT = position_pid((trg_dg/ENCODER_REF), cur_dg, 1);
        Auto_Deg_ctrl();
    }
}

void sys_cotroller(void)
{

    pthread_mutex_lock(&sync_mutex);
    if(sync_flag==2)
    {
        set_ctrl_flag();
        pthread_mutex_lock(&ctrl_mutex2);
        printf("sync_flag %d\n",sync_flag);

        if(Auto_mode_sl)
        {
            Auto_mod();
        }
        else
        {
            manual_mod();
        }

        sync_flag=3;
        pthread_mutex_unlock(&ctrl_mutex2);
    }
    pthread_mutex_unlock(&sync_mutex);
}
