/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_system.h"
#include "HL_het.h"
#include "HL_esm.h"

#include "mpu9250.h"

#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_eqep.h"
#include "HL_etpwm.h"
#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "ipv4/lwip/ip_addr.h"
#include "lwip/udp.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void vTask1(void *pvParameters);
void pidTask(void *pvParameters);
void pwmTask(void *pvParameters);
void udpTask(void *pvParameters);
void sciTask(void *pvParameters);
void mpuTask(void *pvParameters);
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
extern void EMAC_LwIP_Main (uint8_t * emacAddress);

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
void const_velocity(int preCNT, int setCNT, int *error, float c_time);
void fl2in(float a, int *b);
SemaphoreHandle_t xSemaphore = NULL;

/* Define Task Handles */
xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;
xTaskHandle xTask3Handle;
xTaskHandle xTask4Handle;
xTaskHandle xTask5Handle;
xTaskHandle xTask6Handle;
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define SCI_DEBUG   1 // If this value set 1, sciREG1 prints Debug messages.

#if SCI_DEBUG
uint8_t sciTest[] = {"SCI very well\r\n"};
uint8 length = sizeof(sciTest);
uint8_t txtCRLF[] = {'\r', '\n'};

uint8_t txtlwIP[] = {"lwIP Initializing......\n\r"};
uint8_t txtOK[] = {"Success!!!\n\r"};

uint8_t mpuTest[] = {"MPU9250 calibration Success!!!!!!\n\r\0"};
uint8_t mpuInit[] = {"MPU9250 Init Success!!!!!!\n\r\0"};
uint8_t mpuAKInit[] = {"MPU9250 AK8963 Init Success!!!!!!\n\r\0"};
uint8_t mpuGyro[] = {"Gyro_offset_setting Success!!\n\r\0"};
uint8_t mpuYPR[] = {"Init YPR Success!!\n\r\0"};
uint8_t mpuErr[] = {"MPU9250 doesn`t work!!!"};
#endif
/*********************************************************** For PID ***********************************************************************/
int pwm_CMPA;
char buf[32] = {0};
unsigned int buflen = 0;

/* eQEP, PID 변수 */
uint32 pcnt;    // 엔코더 cnt값
float velocity; // 엔코더가 측정한 속도값
float ppr = 3000; // 엔코더 ppr
// ppr = 3000, 감속비 24, Quadrature 모드
// 1 res의 pcnt = 3000 * 24 * 4
// 1 res의 pcnt = 288000
int setCNT = 7200; // 20rpm으로 동작시키기 위한 10ms당 cnt값
int error[2] = {0,0};

// Count 초기화 주기 10ms
float Unit_freq = VCLK3_FREQ * 10000.0;
// eqepSetUnitPeriod(eqepREG1, Unit_freq);

// 엔코더 1ch 펄스당 degree
float ppd = 0.00125; //   360 / (ppr * 24 * 4)
// 제어주기
float c_time = 0.01; //   Unit_freq / (VCLK3_FREQ * 1000000.0)

#define Kp  2.63;
#define Ki  8.4;
#define Kd  0.00015;

float ierr;
float derr;

float P_term;
float I_term;
float D_term;
/*******************************************************************************************************************************************/

/*********************************************************** For MPU ***********************************************************************/
int i;
int apa[2];
int ara[2];
int yd[2];

float real_elev,real_rudd,real_ail,throttle;
float quad_start;

float roll_target_angle = 0.0;
float roll_angle_in;
float roll_rate_in;
float roll_stabilize_kp = 3.5;  // angle kp
float roll_stabilize_ki = 0;  // angle ki
float roll_rate_kp = 0.004;  // angle rate kp setting
float roll_rate_ki = 0.0035;  // angle rate ki setting
float roll_rate_kd = 0.0006;  // angle rate kd setting
float roll_prev_rate_error = 0;
float roll_stabilize_iterm;
float roll_rate_iterm;
float roll_output;

float pitch_target_angle = 0.0;
float pitch_angle_in;
float pitch_rate_in;
float pitch_stabilize_kp = 3.5;
float pitch_stabilize_ki = 0;
float pitch_rate_kp = 0.004;
float pitch_rate_ki = 0.0035;
float pitch_rate_kd = 0.0006;
float pitch_prev_rate_error = 0;
float pitch_stabilize_iterm = 0;
float pitch_rate_iterm = 0;
float pitch_output = 0;

float yaw_target_angle = 0.0;
float yaw_angle_in;
float yaw_rate_in;
float yaw_stabilize_kp = 3.5;
float yaw_stabilize_ki = 0;
float yaw_rate_kp = 0.033;
float yaw_rate_ki = 0.0035;
float yaw_rate_kd = 0;
float yaw_prev_rate_error = 0;
float yaw_stabilize_iterm = 0;
float yaw_rate_iterm = 0;
float yaw_output = 0;

void initYPR(void)
{
    /*for mpu9250*/
    get_YPR();

    init_roll = roll;
    init_pitch = pitch;
    init_yaw = yaw;
    roll_target_angle = init_roll;
    pitch_target_angle = init_pitch;
    yaw_target_angle = init_yaw;

    /*for mpu6050*/
//    get_mpu6050_RPY();
//    roll_target_angle = real_roll;
//    pitch_target_angle = real_pitch;
//    yaw_target_angle = real_yaw;
}
/*******************************************************************************************************************************************/

/* USER CODE END */

uint8	emacAddress[6U] = 	{0x00U, 0x08U, 0xeeU, 0x03U, 0xa6U, 0x6cU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
    /*clear the ESM error manually*/
    esmREG->SR1[2] = 0xFFFFFFFFU;
    esmREG->SSR2   = 0xFFFFFFFF;
    esmREG->EKR = 0x0000000A;
    esmREG->EKR = 0x00000000;

    sciInit();
    i2cInit();
    //VCLK3_FREQ; // 37.500F
    etpwmInit();
    QEPInit();

    uint8 c = readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    sprintf(buf, "I AM = %x\n\r\0", c);
    buflen = strlen(buf);
    sciSend(sciREG1, buflen, (uint8 *)buf);

    if (c == 0x71)
    {
        calibrateMPU9250(gyroBias, accelBias);
        sciSend(sciREG1, sizeof(mpuTest), mpuTest);

        initMPU9250();
        sciSend(sciREG1, sizeof(mpuInit), mpuInit);

        initAK8963(magCalibration);
        sciSend(sciREG1, sizeof(mpuAKInit), mpuAKInit);

        get_offset_value();
        sciSend(sciREG1, sizeof(mpuGyro), mpuGyro);

        initYPR();
        sciSend(sciREG1, sizeof(mpuYPR), mpuYPR);
    }
    else
    {
        sciSend(sciREG1, sizeof(mpuErr), mpuErr);
        while (1)
            ;
    }
    etpwmStartTBCLK();
    eqepEnableCounter(eqepREG1);
    eqepEnableUnitTimer(eqepREG1);

    /* Set high end timer GIO port pin direction to all output */
    gioInit();
    gioSetDirection(gioPORTA, 0xFFFF);
    gioSetDirection(gioPORTB, 0xFFFF);

#if 1
    sciSend(sciREG1, sizeof(txtlwIP), txtlwIP);
    EMAC_LwIP_Main(emacAddress);
    sciSend(sciREG1, sizeof(txtOK), txtOK);
#endif
    // ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT)
    /* Create Task 1 */
    //    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
#if 1
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
#endif
    /* Create Task 2 */
#if 1
    if (xTaskCreate(pidTask,"PID", configMINIMAL_STACK_SIZE, NULL, 6, &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
#endif
    /* Create Task 3 */
#if 0
    if(xTaskCreate(pwmTask, "PWM", configMINIMAL_STACK_SIZE, NULL, 2, &xTask3Handle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 4 */
#if SCI_DEBUG
    if(xTaskCreate(sciTask, "SCI", 2 * configMINIMAL_STACK_SIZE, NULL, 7, &xTask5Handle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 5 */
#if 1
    if(xTaskCreate(udpTask, "UDP", 2 * configMINIMAL_STACK_SIZE, NULL, 8, &xTask4Handle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 6 */
#if 1
    if(xTaskCreate(mpuTask, "MPU", configMINIMAL_STACK_SIZE, NULL, 5, &xTask6Handle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* Task1 */
void vTask1(void *pvParameters)
{
#if 1
    for(;;)
    {
        taskENTER_CRITICAL();
            /* Taggle GIOB[6] with timer tick */
            gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        taskEXIT_CRITICAL();

        vTaskDelay(300);
    }
#endif
}

void pidTask(void *pvParameters)
{
    /*
     * If you want to know position CNT value,
     * Use this Function 'eqepReadPosnCount(eqepREG1)'
     * but value clear every 10ms later.
     */

    for(;;)
    {
        taskENTER_CRITICAL();
        // 10ms마다 플래그 set
        if((eqepREG1->QFLG & 0x800) == 0x800)
        {
            pcnt = eqepReadPosnLatch(eqepREG1); // 정해놓은 시간동안 들어온 CNT 갯수
            velocity = ((float)pcnt * ppd / c_time) / 6.0; // rpm
            const_velocity(pcnt, setCNT, error, c_time);

            // Flag가 자동 초기화가 안됌.
            eqepClearInterruptFlag(eqepREG1, QEINT_Uto);
        }
        taskEXIT_CRITICAL();
        vTaskDelay(10);
    }
}

#if 0
void pwmTask(void *pvParameters)
{
    for(;;)
    {
        taskENTER_CRITICAL();
        if(pwm_CMPA < 3750)
        {
            etpwmREG1->CMPA = 10 * pwm_CMPA++;
        }
        else
            pwm_CMPA = 0;
        taskEXIT_CRITICAL();
        vTaskDelay(10);
    }
}
#endif

#if SCI_DEBUG
void sciTask(void *pvParameters)
{
    /* SCI Baud Rate : 230400
     * sciREG1
     */
#if 1
    sprintf(buf, "\033[H\033[JSCI_Baud_Rate = 230400\n\r\0");
    buflen = strlen(buf);
    sciSend(sciREG1, buflen, (uint8 *)buf);
    sciSend(sciREG1, sizeof(txtCRLF), txtCRLF);
    sciSend(sciREG1, length, sciTest);
#endif

    for(;;)
    {
        taskENTER_CRITICAL();
#if 0
        sprintf(buf, "CMPA = %d\n\r\0", pwm_CMPA);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);

        sprintf(buf, "Velocity = %f\n\r\0", velocity);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);
#endif

#if 1
        sprintf(buf, "CMPA = %d\t setCNT = %d\n\r\0", pwm_CMPA, setCNT);
        buflen = strlen(buf);
        sciSend(sciREG1, buflen, (uint8 *)buf);
//#else
        sprintf(buf, "Velocity = %d\n\r\0", (int)velocity);
        buflen = strlen(buf);
        sciSend(sciREG1, buflen, (uint8 *)buf);
#endif

#if 0
        duty = pwm_CMPA * 100 / PWM_freq;
        sprintf(buf, "CMPA = %d\t Duty = %.1f%%\n\r\0", pwm_CMPA, duty);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);

        sprintf(buf, "POSCNT = %d\n\r\0", pcnt);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);

        sprintf(buf, "setCNT = %d,\t duty = %d\n\r\0", setCNT, (pcnt * 100) / setCNT);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);

        sprintf(buf, "Velocity = %f\n\r\0", velocity);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);
#endif

#if 0
        printf("CMPA = %d\n", pwm_CMPA);
        printf("Velocity = %.2f\n", velocity);
#endif
        fl2in(angle_pitch_acc, apa);
        fl2in(angle_roll_acc, ara);
        fl2in(yaw * R2D, yd);

        sprintf(buf, "roll = %d.%d \t pitch = %d.%d \t yaw = %d.%d \n\r\0",
                apa[0],apa[1], ara[0],ara[1], yd[0],yd[1]);

        buflen = strlen(buf);
        sciSend(sciREG1, buflen, (uint8 *) buf);

        taskEXIT_CRITICAL();
        vTaskDelay(500);
    }
}
#endif

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    if (p != NULL)
    {
        udp_sendto(pcb, p, IP_ADDR_BROADCAST, 7777); //dest port
        pbuf_free(p);
    }
}

void udpTask(void *pvParameters)
{
#if 0
    struct tcp_pcb *pcb;
    err_t err;

    pcb = tcp_new();
    LWIP_ASSERT(("httpd_init: tcp_new failed"), pcb != NULL);
    err = tcp_bind(pcb, IP_ADDR_ANY, HTTPD_SERVER_PORT);
    LWIP_ASSERT(("httpd_init: tcp_bind failed: %s", lwip_strerr(err)), err == ERR_OK);
    pcb = tcp_listen(pcb);
    LWIP_ASSERT(("httpd_init: tcp_listen failed"), pcb != NULL);
    /* initialize callback arg and accept callback */
    tcp_arg(pcb, pcb);
    tcp_accept(pcb, http_accept);
#endif
    struct udp_pcb *pcb;
    char msg[] = "udp test\r\n";
    struct pbuf *p;
    err_t err;

#if 0
    xSemaphoreTake(xSemaphore, (TickType_t) 10);
    if(xSemaphore)
    {
        pcb = udp_new();
        err = udp_bind(pcb, IP_ADDR_ANY, 7777);
        udp_recv(pcb, udp_echo_recv, NULL);
        xSemaphoreGive(xSemaphore);
    }
#else

    pcb = udp_new();
    udp_bind(pcb, IP_ADDR_ANY, 7777);
    udp_recv(pcb, udp_echo_recv, NULL);

#endif

    for(;;)
    {
        taskENTER_CRITICAL();

        p = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM);
        memcpy(p->payload, msg, sizeof(msg));
        udp_sendto(pcb, p, IP_ADDR_BROADCAST, 7777);
        pbuf_free(p);

        taskEXIT_CRITICAL();

        vTaskDelay(500);
        //vTaskDelay(200);
    }
}

void const_velocity(int preCNT, int setCNT, int *error, float c_time)
{
    error[0] = setCNT - preCNT;
    ierr += (float)error[0] * c_time;
    derr = (float)(error[0] - error[1]) / c_time;
    P_term = (float)error[0] * Kp;
    I_term = ierr * Ki;
    D_term = derr * Kd;

    error[1] = error[0];

    pwm_CMPA = (int)(P_term + I_term + D_term);

    if(pwm_CMPA < 0)
    {
        pwm_CMPA = -pwm_CMPA;
    }
    if(pwm_CMPA > 37500)
    {
        pwm_CMPA = 37500;
    }
    etpwmREG1->CMPA = pwm_CMPA;
}

void fl2in(float a, int *b)
{
    b[0] = a;
    if(b[0] > a)
    {
        b[1] = 100000 * (b[0] - a);
    }
    else
        b[1] = 100000 * (a - b[0]);
}

void mpuTask(void *pvParameters)
{
    for (;;)
    {
        taskENTER_CRITICAL();

//         get_mpu6050_RPY();
//         get_mpu6050_gyro();

        if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
        {
            readAccelData(accelCount);
            getAres();

            ax = (float) accelCount[0] * aRes; // - accelBias[0];  // get actual g value, this depends on scale being set
            ay = (float) accelCount[1] * aRes; // - accelBias[1];
            az = (float) accelCount[2] * aRes; // - accelBias[2];

            readGyroData(gyroCount);  // Read the x/y/z adc values
            getGres();

            // Calculate the gyro value into actual degrees per second
            gx = (float) gyroCount[0] * gRes; // get actual gyro value, this depends on scale being set
            gy = (float) gyroCount[1] * gRes;
            gz = (float) gyroCount[2] * gRes;

            get_YPR();
        }
        taskEXIT_CRITICAL();
        vTaskDelay(10);
    }
}
/* USER CODE END */
