#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include "dsp_total.h"
#include "udp_com.h"
#include "vcp_can.h"
#include "bluetooth.h"
#include "ctrl.h"

#define NUM_OF_THREAD 4
#define BUFF_SIZE 32
#define UDP_PORT 7777


void pthread_list(void);
void *serial_thread(void *x);
void *can_thread(void *x);
void *udp_thread(void *x);
void *ctrl_task(void *x);
void emergency(int signo);
void usr_pthread_list(void);
int lidar_data_antinoise(int ld_dat);

int sys_ins_flag =0;

int sync_flag =0;
pthread_mutex_t sync_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t sync_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t sync_cond2 = PTHREAD_COND_INITIALIZER;

pthread_t t_id[NUM_OF_THREAD];
void *usr_t_list[NUM_OF_THREAD]= {NULL,};

int main(void)
{
    int thread_cnt =0;
    int pcancel_oldstate;
    int pcancel_oldtype;
    sys_ins_flag=0;
    sync_flag=0;

    pthread_mutex_init(&sync_mutex,NULL);
    pthread_cond_init(&sync_cond,NULL);
    pthread_cond_init(&sync_cond2,NULL);

    signal(SIGINT, emergency);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &pcancel_oldstate);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &pcancel_oldtype);

    usr_pthread_list();

    for(thread_cnt=0; thread_cnt<NUM_OF_THREAD;thread_cnt++)
    {
        if(pthread_create(&t_id[0], NULL, usr_t_list[thread_cnt], NULL)<0)
         {
            perror("error:");
            exit(0);
         }
    }

    for(thread_cnt=0 ;thread_cnt<NUM_OF_THREAD; thread_cnt++)
        pthread_join(t_id[thread_cnt],NULL);

    pthread_mutex_destroy(&sync_mutex);
    pthread_cond_destroy(&sync_cond);
    pthread_cond_destroy(&sync_cond2);
    return 0;
}

void usr_pthread_list(void)
{
    usr_t_list[0] = udp_thread;
    usr_t_list[1] = serial_thread;
    usr_t_list[2] = can_thread;
    usr_t_list[3] = ctrl_task;
}

void emergency(int signo)
{
    int em_cnt =0;
    close_udp();
    close_can();
    close_bl_dv();
    close_ctrl();

    for(em_cnt=0;em_cnt<NUM_OF_THREAD;em_cnt++)
        pthread_cancel(t_id[em_cnt]);

    pthread_mutex_destroy(&sync_mutex);
    pthread_cond_destroy(&sync_cond);
    pthread_cond_destroy(&sync_cond2);

    printf("\r\n *exit* \r\n");
    sleep(3);
    exit(0);
}

void *serial_thread(void *x)
{
    int bl_cnt =0;
    printf("init_start bl\r\n");
    init_bl();
    printf("init_success bl\r\n");
    usleep(10);
    while(1)
    {
        recieve_bl();

        if(read_bl_ins>0)
        {
            if(read_bl_ins==1)
            {
                printf("\r\n left \n");
            }
            else if(read_bl_ins==2)
            {
                printf("\r\n up \n");
            }
            else if(read_bl_ins==3)
            {
                printf("\r\n right \n");
            }
            else if(read_bl_ins==4)
            {
                printf("\r\n down \n");
            }
            else if(read_bl_ins==5)
            {
                printf("\r\n laser \n");
            }
            else if(read_bl_ins==6)
            {
                printf("\r\n shoot \n");
            }
            else if(read_bl_ins==7)
            {
                printf("\r\n mode \n");
            }
            else
            {
                printf("start\n");
            }

            pthread_mutex_lock(&ctrl_mutex);
            ct_flag = read_bl_ins;
            pthread_mutex_unlock(&ctrl_mutex);

            read_bl_ins = 0;
        }

        usleep(1);
    }

    close_bl_dv();
    pthread_exit(0);
}


void *ctrl_task(void *x)
{
    init_ctrl();
    usleep(10);
    while(1)
    {
        sys_cotroller();
        usleep(10);
    }

    close_ctrl();
    pthread_exit(0);
}

int lidar_data_antinoise(int ld_dat)
{
    int ld_res =0;
    static int old_dat[3]= {0,0,0};

    if(ld_dat==0)
        ld_res = (5*old_dat[0] + 3*old_dat[1] +  2*old_dat[2])/10;
    else
        ld_res = ld_dat;

    old_dat[2] =old_dat[1];
    old_dat[1] =old_dat[0];
    old_dat[0] =ld_res;

    return ld_res;
}


void *can_thread(void *x)
{
    volatile int can_success =-1;
    init_can();
    usleep(10);

    while(1)
    {
        pthread_mutex_lock(&sync_mutex);
        if(sync_flag ==1)
        {
            printf("sync_flag %d\n",sync_flag);
            while(can_success == -1)
            {
                can_success = Recieve_fpga_data();
                usleep(100);
            }

            if(can_success==0)
            {
                 distance = lidar_data_antinoise(lidar_data);
                 cur_dg = encoder_data;

                 printf("lidar = %d\t", distance);
                 printf("encoder = %d\n",encoder_data);

                 lidar_data = -1;
                 encoder_data = -1;
                 sync_flag =2;
                 can_success = -1;
             }
        }
        pthread_mutex_unlock(&sync_mutex);
        usleep(10);
    }

    close_can();
    pthread_exit(0);
}

int recieve_stack =0;

void *udp_thread(void *x)
{
    int udp_cnt =0;
    sockad_in   server_addr;
    sockad_in   client_addr;

    int mcu_set_cnt =0;
    int mcu_set_deg =0;
    static int laser_stack =0;


    static int old_mcu_set_deg =0;
    static int old_read_mcu_state=0;

    printf("init_udp_start\r\n");
    init_udp(&server_addr, &client_addr, UDP_PORT);
    printf("init_udp_success\r\n");

    usleep(1000);

    while(1)
    {
        Udp_Receive2MCU(&client_addr);
        pthread_mutex_lock(&sync_mutex);
        if(read_mcu_state)
        {

            if(sync_flag==0)
            {
                printf("sync_flag %d\n",sync_flag);
                sync_flag =1;
            }
            else if(sync_flag==3)
            {

                pthread_mutex_lock(&ctrl_mutex2);
                printf("sync_flag %d\n",sync_flag);

                mcu_set_cnt = setCNT;
                if((read_mcu_state & 0x02)>>1)
                {
                    mcu_set_deg = degree;
                    printf("\n deg %d\r\n", mcu_set_deg);
                    printf("oid deg %d\r\n\n", old_mcu_set_deg);
                }

                if((mcu_set_deg != old_mcu_set_deg) && (recieve_stack==2))
                    recieve_stack =1;

                else if(((shoot_flag) || (laser_flag)) && ((read_mcu_state & 0x03)==0x03))
                    shoot_ready_flag =1;
                else if((shoot_ready_flag==1) && ((old_read_mcu_state & 0x01)==0x01) && ((read_mcu_state & 0x01)==0x00))
                {
                    shoot_fin_flag =1;
                    shoot_ready_flag =0;
                }


                if((recieve_stack==0) && ((read_mcu_state & 0X04)>>2))
                {
                    Udp_Send2MCU(&client_addr,STR_INS,mcu_set_cnt,mcu_set_deg); //start mcu g instrument
                    recieve_stack =1;
                }
                else if((recieve_stack ==1) && ((read_mcu_state & 0x02)>>1))
                {
                    Udp_Send2MCU(&client_addr,DEG_INS,mcu_set_cnt,mcu_set_deg);
                    old_mcu_set_deg = mcu_set_deg;
                    recieve_stack =2;
                }
                else if(shoot_ready_flag)
                {
                    if(shoot_flag)
                    {
                        Udp_Send2MCU(&client_addr,SHT_INS,mcu_set_cnt,mcu_set_deg);
                        shoot_flag =0;
                    }
                    else if(laser_flag)
                    {
                        if(laser_stack==0)
                        {
                        Udp_Send2MCU(&client_addr,LASER_INS,mcu_set_cnt,mcu_set_deg);
                        laser_stack =1;
                        }
                        else
                        {
                            Udp_Send2MCU(&client_addr,LASER_OFF_INS,mcu_set_cnt,mcu_set_deg);
                            laser_stack=0;
                        }

                        laser_flag =0;
                        shoot_ready_flag=0;
                    }
                }
                else
                    Udp_Send2MCU(&client_addr,STD_INS,mcu_set_cnt,mcu_set_deg);

                old_read_mcu_state = read_mcu_state;
                read_mcu_state = 0;
                sync_flag =0;
                pthread_mutex_unlock(&ctrl_mutex2);
            }
        }
        pthread_mutex_unlock(&sync_mutex);
        usleep(10);
    }

    close_udp();
    pthread_exit(0);
}
