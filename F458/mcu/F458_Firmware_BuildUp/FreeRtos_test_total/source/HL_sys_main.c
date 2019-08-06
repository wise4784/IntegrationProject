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

#define UART                    sciREG1
#define LIDAR_SLAVE_ADDR        0x62

#define ACQ_COMMAND             0x00
#define STATUS                  0x1
#define SIG_COUNT_VAL           0x2
#define ACQ_CONFIG_REG          0x4
#define THRESHOLD_BYPASS        0x1c
#define READ_FROM               0x8f

#define FULL_DELAY_HIGH         0x0f
#define DCNT                    6

volatile int flag;
volatile int flag_lidar=0;
int throttle,rudder,data;
/*char buf[128]={0};//task1 use
unsigned int buf_len;//task1 use
char buf2[128]={0};//task2 use
unsigned int buf_len2;//task2 use*/

char buf0[128]={0};//only lidar use
unsigned int buf_len0;//only lidar use
uint16 tmp;//only lidar use

char buf1[128]={0};//only bldc use
unsigned int buf_len1;//only bldc use

char buf2[128]={0};//only servo use
unsigned int buf_len2;//only servo use

char buf3[128]={0};//only CMPA use
unsigned int buf_len3;//only CMPA use

uint8 receives[2];
uint8 bias_cnt;
uint8_t rx_data[DCNT]={0};

void Init_all(void);
void lidar_enable(void);
void sci_display(sciBASE_t *sci, uint8 *test, uint32 len);
void disp_set(char *str);
void wait(uint32);
void get_data(void);
void lidar_bias(void);
void use_lidar(void);
void ecap_throttle(void);//ecap_control pwm
void ecap_servo(void);//ecap_control pwm
void bldc_back(void);//bldc back throttle
void bldc_back_low(void);//bldc back low throttle
void servo_control(void);//servo rudder control pwm

void task0(void *);
void task1(void *);
void task2(void *);
void task3(void *);

SemaphoreHandle_t xSemaphore;
TaskHandle_t i2c_handle;
TaskHandle_t bldc_handle;
TaskHandle_t servo_handle;
TaskHandle_t sci_handle;

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    Init_all();
    disp_set("!!!!!!!!!!!!!!!!!!Init_all complete!!!!!!!!!!!!!!!!!");
    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);//semaphore initial

    if(xTaskCreate(task0, "i2c",configMINIMAL_STACK_SIZE*2, NULL, 5,&i2c_handle)!=pdPASS)
    {
        for(;;)
            ;
    }

    if(xTaskCreate(task1, "bldc", configMINIMAL_STACK_SIZE*2, NULL, 5, &bldc_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(task2, "servo", configMINIMAL_STACK_SIZE*2, NULL, 5, &servo_handle)!=pdPASS)
    {
        for(;;)
            ;
    }

    if(xTaskCreate(task3, "sci", configMINIMAL_STACK_SIZE*2, NULL, 5, &sci_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    vTaskStartScheduler();
/* USER CODE END */
    return 0;
}

/* USER CODE BEGIN (4) */
void task0(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            use_lidar();
            if(tmp<=50)
            {
                vTaskSuspend(bldc_handle);
                vTaskSuspend(servo_handle);
                //wait(1000000);
                etpwmREG1->CMPA = 1550;
                etpwmREG2->CMPA = 950;
                flag_lidar = 1;
            }
            if(tmp>50 && flag_lidar == 1 )
            {
                vTaskResume(bldc_handle);
                vTaskResume(servo_handle);
                etpwmREG1->CMPA = 1500;
                etpwmREG2->CMPA = 1250;
                //wait(1000000);
                flag_lidar = 0;
            }


            switch(rx_data[0])
            {
            case 13://can start data
                vTaskSuspend(bldc_handle);
                vTaskSuspend(servo_handle);
                //vTaskSuspend(sci_handle);
                data = rx_data[1] * 1000 + rx_data[2] * 100 + rx_data[3] * 10 + rx_data[4];
                etpwmREG1->CMPA = data;
                etpwmREG2->CMPA = 1250;
                break;
            case 12:
                vTaskResume(bldc_handle);
                vTaskResume(servo_handle);
                //vTaskResume(sci_handle);
                break;
            default:
                break;
            }
          // wait(100000);
           memset(rx_data, 0, sizeof(rx_data));
           data = 0;
           xSemaphoreGive(xSemaphore);
           vTaskDelay(30);
            }
        }
    }

void task1(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            ecap_throttle();
            if((throttle>=1200 && throttle<1450))
            {
                bldc_back();
            }else if(throttle<1200 && throttle>=900)
            {
                bldc_back_low();
            }else if(throttle<900)
            {
                etpwmREG1->CMPA = 1500;
            }else if(throttle>=1450 && throttle<=1550)
            {
                etpwmREG1->CMPA = 1500;
            }else if(throttle>1550 && throttle<=1800)
            {
                etpwmREG1->CMPA = throttle;
            }else
            {
                etpwmREG1->CMPA = 1800;
            }
            xSemaphoreGive(xSemaphore);
            vTaskDelay(30);
        }
    }
}

void task2(void *pvPrameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            ecap_servo();
            servo_control();

            xSemaphoreGive(xSemaphore);
            vTaskDelay(30);
        }
    }
}

void task3(void *pvPrameters)
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            sprintf(buf0, "Distance = %d\n\r\0", tmp);
            buf_len0 = strlen(buf0);
            sci_display(sciREG1, (uint8 *)buf0, buf_len0);
            sprintf(buf1, "bldc motor = %d ms\n\r\0", throttle);
            buf_len1 = strlen(buf1);
            sci_display(UART, (uint8 *)buf1, buf_len1);
            sprintf(buf2, "servo motor = %d ms\n\r\0", rudder);
            buf_len2 = strlen(buf2);
            sci_display(UART, (uint8 *)buf2, buf_len2);
            sprintf(buf3, "etpwm = %d ms\n\r\0", etpwmREG1->CMPA);
            buf_len3 = strlen(buf3);
            sci_display(UART, (uint8 *)buf3, buf_len3);



            xSemaphoreGive(xSemaphore);
             vTaskDelay(30);
        }
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
    canInit();
    canEnableErrorNotification(canREG1);
    lidar_enable();
    wait(100000);
}

void lidar_enable(void)
{
    uint8 tmp[4] = {0x80, 0x08, 0x00, 0x04};

    volatile unsigned int cnt = 7;
    i2cSetSlaveAdd(i2cREG2, LIDAR_SLAVE_ADDR); //lidar slave config
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);

    i2cSendByte(i2cREG2, SIG_COUNT_VAL);
    i2cSend(i2cREG2, 1, &tmp[0]);

    i2cSendByte(i2cREG2, ACQ_CONFIG_REG);
    i2cSend(i2cREG2, 1, &tmp[1]);

    i2cSendByte(i2cREG2, THRESHOLD_BYPASS);
    i2cSend(i2cREG2, 1, &tmp[2]);

    i2cSendByte(i2cREG2, ACQ_CONFIG_REG);
    i2cSend(i2cREG2, 1, &tmp[3]);

    //disp_set("Lidar Default Initial Success!!!\n\r\0");

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

    while(i2cIsBusBusy(i2cREG2)==true)
        ;
    while(i2cIsStopDetected(i2cREG2)==0)
        ;

    i2cClearSCD(i2cREG2);

    i2cSetDirection(i2cREG2, I2C_RECEIVER);
    i2cSetCount(i2cREG2, 2);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStart(i2cREG2);
    i2cReceive(i2cREG2, 2, (unsigned char *)receives);
    i2cSetStop(i2cREG2);

    while(i2cIsBusBusy(i2cREG2)==true)
        ;
    while(i2cIsStopDetected(i2cREG2)==0)
        ;

    i2cClearSCD(i2cREG2);

    flag=1;

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
    while(i2cIsStopDetected(i2cREG2)==0)
        ;

    i2cClearSCD(i2cREG2);

    wait(100000);
}

void use_lidar(void)
{

        get_data();
        bias_cnt++;

        tmp = receives[0] << 8;
        tmp = receives[1];

        if(bias_cnt == 50 && flag == 1)
        {
            lidar_bias();
            bias_cnt = 0;

            flag = 0;
        }
}

void sci_display(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((UART->FLR & 0x04)==4)
            ;
        sciSendByte(UART, *text++);
    }
}

void disp_set(char *str)
{
    char buf[128]={0};
    unsigned int buf_len;
    sprintf(buf, str);
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
    wait(100000);
}

void wait(uint32 delay)
{
    int i;
    for(i=0; i<delay; i++)
        ;
}

void ecap_throttle(void)
{
    unsigned int cap[2];

    cap[0] = ecapGetCAP1(ecapREG3);
    cap[1] = ecapGetCAP2(ecapREG3);
    throttle = (cap[1] - cap[0])/VCLK3_FREQ;

}

void ecap_servo(void)
{
   unsigned int cap[2];

   cap[0] = ecapGetCAP1(ecapREG2);
   cap[1] = ecapGetCAP2(ecapREG2);
   rudder = (cap[1] - cap[0])/VCLK3_FREQ;
   rudder = 2700 - rudder;
}


void bldc_back(void)
{
    etpwmREG1->CMPA = 1500;
    wait(250000);
    etpwmREG1->CMPA = 1000;
    wait(250000);
    etpwmREG1->CMPA = 1500;
    wait(250000);
    etpwmREG1->CMPA = throttle;
}

void bldc_back_low(void)
{
    etpwmREG1->CMPA = 1500;
    wait(250000);
    etpwmREG1->CMPA = 1000;
    wait(250000);
    etpwmREG1->CMPA = 1500;
    wait(250000);
    etpwmREG1->CMPA = 1200;
}

void servo_control(void)
{
    if(rudder>=900 && rudder<1200)
    {
        etpwmREG2->CMPA = rudder;
    }else if(rudder < 900)
    {
        etpwmREG2->CMPA = 900;
    }else if(rudder>=1200 && rudder<=1300)
    {
        etpwmREG2->CMPA = 1250;
    }else if(rudder>1300 && rudder<=1800)
    {
        etpwmREG2->CMPA = rudder;
    }else if(rudder>1800 && rudder<=2000)
    {
        etpwmREG2->CMPA = 1800;
    }else
    {
        etpwmREG2->CMPA = 1250;
    }
}

void canMessageNotification(canBASE_t *node, uint32_t messageBox)
{
    while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2))
        ;
    canGetData(canREG1, canMESSAGE_BOX2, (uint8 *)&rx_data[0]);

#if 0
    switch(rx_data[0])
    {
    case 13://can start data
        vTaskSuspend(bldc_handle);
        vTaskSuspend(servo_handle);
        //vTaskSuspend(sci_handle);
        data = rx_data[1] * 1000 + rx_data[2] * 100 + rx_data[3] * 10 + rx_data[4];
        etpwmREG1->CMPA = data;
        etpwmREG2->CMPA = 1250;

        break;
    case 12:
        vTaskResume(bldc_handle);
        vTaskResume(servo_handle);
        //vTaskResume(sci_handle);

        break;
    default:
        break;
    }

   // wait(100000);

    memset(rx_data, 0, sizeof(rx_data));
    data = 0;*/
#endif
}
/* USER CODE END */
