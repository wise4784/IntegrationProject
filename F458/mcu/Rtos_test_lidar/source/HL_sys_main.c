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
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "os_task.h"
#include "os_queue.h"
#include "os_semphr.h"

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#include "HL_sci.h"
#include "HL_etpwm.h"
#include "HL_ecap.h"
#include "HL_can.h"
#include "HL_i2c.h"
#include "HL_system.h"
#include "HL_esm.h"
// #include "HL_system_core.h"

#define UART    sciREG1
#define LIDAR_SLAVE_ADDR 0x62

#define ACQ_COMMAND 0x00
#define STATUS  0x1
#define SIG_COUNT_VAL 0x2
#define ACQ_CONFIG_REG  0x4
#define THRESHOLD_BYPASS    0x1c
#define READ_FROM   0x8f

#define FULL_DELAY_HIGH 0x0f
#define DCNT    8

uint8 receives[2];

volatile int flag;

uint8 bias_cnt;
int throttle;
int rudder;

void Init_all(void);
void lidar_enable(void);
void sci_display(sciBASE_t *sci, uint8 *test, uint32 len);
//void pwm_set(void);
void wait(uint32);

void disp_set(char *str);

//void lidar_without_bias(void);
void lidar_bias(void);
void get_data(void);
void bldc_back(void);
void blde_back_low(void);

void task0(void *);
void task1(void *);
void task2(void *);

char buf[128] = {0};
unsigned int buf_len;

uint8_t rx_data[DCNT] = {0};

//volatile int i;
//int cnt = 1;
//uint16 avg[2] = {0};

SemaphoreHandle_t xSemaphore;
TaskHandle_t i2c_handle;
TaskHandle_t bldc_handle;
TaskHandle_t servo_handle;

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    Init_all();

    disp_set("Init_all complete!!!!!!!!!\n\r\0");
    wait(100000);

    //lidar_enable();
    //disp_set("lidar ready complete!!!!!!!!\n\r\0");
    //wait(100000);

    xSemaphore = xSemaphoreCreateBinary();// cpu 공유자원이라고 하면.
    xSemaphoreGive(xSemaphore);

    if(xTaskCreate(task0,"i2c",configMINIMAL_STACK_SIZE*2, NULL, 1,&i2c_handle)!=pdPASS)
    {
        for(;;)
            ;
    }

   if(xTaskCreate(task1, "bldc",configMINIMAL_STACK_SIZE*2, NULL, 1,&bldc_handle)!=pdPASS)
    {
        for(;;)
            ;
    }

    if(xTaskCreate(task2, "servo", configMINIMAL_STACK_SIZE*2, NULL, 1, &servo_handle)!=pdPASS)
    {
        for(;;)
            ;
    }



    vTaskStartScheduler();

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void task0(void *pvParameters) //lidar&i2c
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
                get_data();
                bias_cnt++;

                if(bias_cnt == 10 && flag ==1)
                {
                    uint16 tmp;
                    lidar_bias();
                    bias_cnt = 0;

                    tmp = receives[0] << 8;
                    tmp |= receives[1];

                    sprintf(buf, "Distance = %d\n\r\0", tmp);
                    buf_len = strlen(buf);
                    sci_display(sciREG1, (uint8 *)buf, buf_len);

                    if(tmp <= 50)// car stop state  ecap control
                    {
                         vTaskSuspend(bldc_handle);
                         vTaskSuspend(servo_handle);
                         disp_set("bldc&&&&servo stop!!!!!!!!!\n\r\0");
                         wait(100000000);

                         etpwmREG2->CMPA = 1000;
                         etpwmREG1->CMPA = 1560;

                     }
                        if(tmp>50)
                        {
                            vTaskResume(bldc_handle);
                            vTaskResume(servo_handle);
                        }
                        flag =  0;
                }
                xSemaphoreGive(xSemaphore);
                vTaskDelay(1);
            }
        }
    }

void task1(void *pvParameters)//bldc motor control
{
    unsigned int cap[2];

    for(;;)
    {
        if(xSemaphoreTake(xSemaphore,(TickType_t)0x01)==pdTRUE)
        {
            cap[0] = ecapGetCAP1(ecapREG3);
            cap[1] = ecapGetCAP2(ecapREG3);
            throttle = (cap[1] - cap[0])/VCLK3_FREQ; //channel 3
            sprintf(buf, "bldc motor = %d ms\n\r", throttle);
            buf_len = strlen(buf);
            sci_display(UART, (uint8 *)buf, buf_len);

            if((1200 <= throttle && throttle < 1450)|| throttle <1200)//bldc_motor back
            {
                bldc_back();
                //etpwmREG1->CMPA = throttle;
            }
            else if(1200> throttle)//bldc_motor back
            {
                 blde_back_low();
            }

            else if(1450 <= throttle && throttle <= 1550) //bldc_motor neutrality
            {
                etpwmREG1->CMPA = 1500;
            }
            else if(1550 < throttle && throttle <= 1800) //bldc_motor forward
            {
                etpwmREG1->CMPA = throttle;
            }
            else//bldc_motor forward
            {
                etpwmREG1->CMPA = 1800;
            }
            xSemaphoreGive(xSemaphore);
            vTaskDelay(1);
        }
    }
}

void task2(void *pvParameters)
{
    char buf[128] = {0};
    unsigned int buf_len;
    unsigned int cap[2];

    for(;;)
    {
        if(xSemaphoreTake(xSemaphore,(TickType_t)0x01)==pdTRUE)
        {
            cap[0] = ecapGetCAP1(ecapREG2);
            cap[1] = ecapGetCAP2(ecapREG2);
            rudder = (cap[1] - cap[0])/VCLK3_FREQ; //channel 2
            sprintf(buf, "servo motor = %d ms\n\r", rudder);
            buf_len = strlen(buf);
            sci_display(UART, (uint8 *)buf, buf_len);

            if(rudder<1000)//servo_motor left
            {
                etpwmREG2->CMPA = 900;
            }
            else if(1000<=rudder && 1450>rudder)//servo_motor left
            {
                etpwmREG2->CMPA = rudder;
            }
            else if(1450<=rudder && 1550>=rudder)//servo_motor neutrality
            {
                etpwmREG2->CMPA = 1250;
            }
            else if(1550<rudder && 1800>=rudder)//servo_motor right
            {
                etpwmREG2->CMPA = rudder;
            }
            else//servo_motor right
            {
                etpwmREG2->CMPA = 1800;
            }
            xSemaphoreGive(xSemaphore);
            vTaskDelay(1);
        }
    }
}


void lidar_enable (void)
{
    uint8 tmp[4] = {0x80, 0x08, 0x00, 0x04};//한번에 처리

       volatile unsigned int cnt = 7;//volatile 없으면 최적화 시키는데  값을 멋대로 입력되어질수가 있음. 값이 변하는 변수에는 volatile 사용 메모리에서 읽어서 값을 써라(특히 리눅스에서 명확히 해야함)
       i2cSetSlaveAdd(i2cREG2, LIDAR_SLAVE_ADDR); //lidar slave config
       i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
       i2cSetStop(i2cREG2);
       i2cSetStart(i2cREG2);

       i2cSendByte(i2cREG2, SIG_COUNT_VAL);// 맥시멈액큐시 레지스터 값 설정
       i2cSend(i2cREG2, 1, &tmp[0]);

       i2cSendByte(i2cREG2, ACQ_CONFIG_REG);//주소 . 상관계수 :
       i2cSend(i2cREG2, 1, &tmp[1]);//4번레지스터에 초기값 0x08 설정. 스캔 처리 속도 세팅

       i2cSendByte(i2cREG2, THRESHOLD_BYPASS);// 값 0x20,0x60 과의 설정값 적절하게
       i2cSend(i2cREG2, 1, &tmp[2]);

       i2cSendByte(i2cREG2, ACQ_CONFIG_REG);//0x40 값을 보정
       i2cSend(i2cREG2, 1, &tmp[3]);

       disp_set("Lidar Default Initial Success!!!\n\r\0");

       while(i2cIsBusBusy(i2cREG2) == true)
                   ;
       while(i2cIsStopDetected(i2cREG2) == 0)
                   ;

       i2cClearSCD(i2cREG2);

      wait(100000);

}

void get_data(void)
{
    i2cSetSlaveAdd(i2cREG2, LIDAR_SLAVE_ADDR);
        i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
        i2cSetCount(i2cREG2, 1);
        i2cSetMode(i2cREG2, I2C_MASTER);
        i2cSetStop(i2cREG2);
        i2cSetStart(i2cREG2);
        i2cSendByte(i2cREG2, READ_FROM);

        while(i2cIsBusBusy(i2cREG2) == true)
            ;
        while(i2cIsStopDetected(i2cREG2) == 0)
            ;

        i2cClearSCD(i2cREG2);

        i2cSetDirection(i2cREG2, I2C_RECEIVER);
        i2cSetCount(i2cREG2, 2);
        i2cSetMode(i2cREG2, I2C_MASTER);
        i2cSetStart(i2cREG2);
        i2cReceive(i2cREG2, 2, (unsigned char *)receives);
        i2cSetStop(i2cREG2);

        while(i2cIsBusBusy(i2cREG2) == true)
            ;
        while(i2cIsStopDetected(i2cREG2) == 0)
            ;

        i2cClearSCD(i2cREG2);

        flag = 1;
}

void lidar_bias(void)
{
    volatile unsigned int cnt = 1;
    unsigned char data[1] = {0x04U};

    i2cSetSlaveAdd(i2cREG2, LIDAR_SLAVE_ADDR);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    i2cSetCount(i2cREG2, cnt + 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, ACQ_COMMAND);
    i2cSend(i2cREG2, cnt, data);
    i2cSetStop(i2cREG2);

    while(i2cIsBusBusy(i2cREG2) == true)
            ;
    while(i2cIsStopDetected(i2cREG2) == 0)
            ;

    i2cClearSCD(i2cREG2);

    wait(100000);

}

void canMessageNotification(canBASE_t *node, uint32_t notification)
{
    if(node == canREG1)
    {
        int p;
        while(!canIsRxMessageArrived(canREG1,canMESSAGE_BOX2))
            ;
        canGetData(canREG1, canMESSAGE_BOX2, (uint8 *)&rx_data[0]);
        for(p=0; p<8; p++)
            printf("rx_data: %x\n", rx_data[p]);
    }
}


void Init_all(void)
{
    sciInit();
    ecapInit();
    ecapStartCounter(ecapREG2);
    ecapStartCounter(ecapREG3);
    ecapEnableCapture(ecapREG2);
    ecapEnableCapture(ecapREG3);
    etpwmInit();
    etpwmStartTBCLK();
    i2cInit();
    lidar_enable();
    canInit();
    canEnableErrorNotification(canREG1);
}

void bldc_back(void)
{
    etpwmREG1->CMPA = 1500;
    wait(100000);
    etpwmREG1->CMPA = 1000;
    wait(100000);
    etpwmREG1->CMPA = 1500;
    wait(100000);
    etpwmREG1->CMPA = throttle;
}

void blde_back_low(void)
{
    etpwmREG1->CMPA = 1500;
    wait(100000);
    etpwmREG1->CMPA = 1000;
    wait(100000);
    etpwmREG1->CMPA = 1500;
    wait(100000);
    etpwmREG1->CMPA = 1200;
}
void wait(uint32 delay)
{
    int i;

    for(i = 0; i < delay; i++)
        ;
}

void sci_display(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((UART->FLR & 0x04) == 4)
                ;
        sciSendByte(UART, *text++);
    }
}

void disp_set(char *str)
{
    char buf[128] = {0};
    unsigned int buf_len;
    sprintf(buf, str);
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
    wait(100000);
}
/* USER CODE END */
