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
#include "HL_gio.h"
#include "HL_etpwm.h"
#include "HL_ecap.h"
#include "HL_can.h"
#include "HL_eqep.h"
#include "HL_system.h"
#include "HL_esm.h"

#define UART                    sciREG1
#define DCNT                    8
#define UnitPeriod              10000000

volatile int flag;
volatile int flag_lidar= 0;
volatile int flag_output = 0;
volatile int flag_toggle = 0;
volatile int flag_led = 0;
volatile int flag_accelerate = 0;

int throttle,rudder,data,output;
int dsp_throttle, dsp_rudder=0;
int front_led, rear_led, left_blink, right_blink, blink_led, emergency_light;



/*char buf0[64]={0};//toggle switch output
unsigned int buf_len0;//toggle switch output

char buf1[64]={0};//bldc throttle
unsigned int buf_len1;//bldc throttle

char buf2[64]={0};//servo rudder
unsigned int buf_len2;//servo rudder

char buf3[64]={0};//etpwmREG1->CMPA use
unsigned int buf_len3;//etpwmREG1->CMPA use

char buf4[64]={0};//etpwmREG2->CMPA use
unsigned int buf_len4;//etpwmREG2->CMPA use
*/

uint8_t tx_data[8]={0};
uint8_t rx_data[DCNT]={0};
uint8_t receive[5]={0x01,0x2c};
uint16_t lidar=300;
int deltaT=0;;
int deltapos=0;
volatile uint16_t flag_eqep;
volatile uint8_t dsp_flag;

void Init_all(void);
void sci_display(sciBASE_t *sci, uint8 *test, uint32 len);
void disp_set(char *str);
void wait(uint32);
void ecap_throttle(void);//ecap_control pwm
void ecap_servo(void);//ecap_control pwm
void ecap_toggle(void);//ecap_toggle switch
void ecap_channel5(void);
void ecap_channel6(void);
void ecap_channel8(void);
void bldc_back(void);//bldc back throttle
void bldc_back_low(void);//bldc back low throttle
void servo_control(void);//servo rudder control pwm

void task0(void *);
void task1(void *);
void task2(void *);
//void task3(void *);
void task4(void *);
void task5(void *);
void task6(void *);
void task7(void *);

SemaphoreHandle_t xSemaphore;
TaskHandle_t dsp_can_handle;
TaskHandle_t bldc_handle;
TaskHandle_t servo_handle;
TaskHandle_t sci_handle;
TaskHandle_t fpga_can_handle;
TaskHandle_t toggle_handle;
TaskHandle_t led_handle;
TaskHandle_t eqep_handle;

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    Init_all();

    disp_set("!!!!!!!!!!!!!!!!!!Init_all complete!!!!!!!!!!!!!!!!!");

    xSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(xSemaphore);//semaphore initial

    if(xTaskCreate(task0, "can1",configMINIMAL_STACK_SIZE*2, NULL, 5,&dsp_can_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(task1, "bldc", configMINIMAL_STACK_SIZE, NULL, 5, &bldc_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(task2, "servo", configMINIMAL_STACK_SIZE, NULL, 5, &servo_handle)!=pdPASS)
    {
        for(;;)
            ;
    }

   /* if(xTaskCreate(task3, "sci", configMINIMAL_STACK_SIZE*2, NULL, 5, &sci_handle)!=pdPASS)
    {
        for(;;)
            ;
    }*/
    if(xTaskCreate(task4, "can2", configMINIMAL_STACK_SIZE*2, NULL, 5, &fpga_can_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(task5, "toggle", configMINIMAL_STACK_SIZE, NULL, 5, &toggle_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(task6, "led", configMINIMAL_STACK_SIZE, NULL, 5, &led_handle)!=pdPASS)
    {
        for(;;)
            ;
    }
    if(xTaskCreate(task7, "eqep", configMINIMAL_STACK_SIZE, NULL, 5, &eqep_handle)!=pdPASS)
    {
        for(;;)
            ;
    }

    vTaskStartScheduler();

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */


void task0(void *pvParameters)//can(canREG1) dsp-mcu task port6
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {

            if(rx_data[0])//dsp_control can interrupt execute
             {

                dsp_throttle = rx_data[1]*1000 + rx_data[2]*100 + rx_data[3]*10 + rx_data[4];

                if(rx_data[5] == 1)
                {
                    dsp_rudder = 900;
                }else if(rx_data[5] == 2)
                {
                    dsp_rudder = 1700;
                }

                //dsp_rudder = rx_data[2]<<8;
               // dsp_rudder |= rx_data[3];


               // rx_data[0]= dsp_throttle >> 8
               //rx_data[1] = dsp_throttle & 0xFF
                //wait(100000);
             }

                xSemaphoreGive(xSemaphore);
                vTaskDelay(30);
        }
    }
}


void task1(void *pvParameters)//ecap-bldc_motor task
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            ecap_throttle();

            if((throttle>=1200 && throttle<1450))
            {
                bldc_back();
            }
            else if(throttle<1200 && throttle>=900)
            {
                bldc_back_low();
            }
            else if(throttle<900)
            {
                etpwmREG1->CMPA = 1500;
            }
            else if(throttle>=1450 && throttle<=1550)
            {
                etpwmREG1->CMPA = 1500;
            }
            else if(throttle>1550 && throttle<=1800)
            {
                etpwmREG1->CMPA = throttle;
            }
            else
            {
                etpwmREG1->CMPA = 1800;
            }
            xSemaphoreGive(xSemaphore);
            vTaskDelay(30);
        }
    }
}

void task2(void *pvPrameters)//ecap-servo_motor task
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
/*
void task3(void *pvPrameters)//sci task
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            sprintf(buf0, "!!!!!!!ecap output!!!!!!!!! = %d\n\r\0", output);
            buf_len0 = strlen(buf0);
            sci_display(UART, (uint8 *)buf0, buf_len0);
            sprintf(buf1, "bldc motor = %d ms\n\r\0", throttle);
            buf_len1 = strlen(buf1);
            sci_display(UART, (uint8 *)buf1, buf_len1);
            sprintf(buf2, "servo motor = %d ms\n\r\0", rudder);
            buf_len2 = strlen(buf2);
            sci_display(UART, (uint8 *)buf2, buf_len2);
            sprintf(buf3, "etpwmREG1->CMPA = %d ms\n\r\0", etpwmREG1->CMPA);
            buf_len3 = strlen(buf3);
            sci_display(UART, (uint8 *)buf3, buf_len3);
            sprintf(buf4, "lidar = %d ms\n\r\0", lidar);
            buf_len4 = strlen(buf4);
            sci_display(UART, (uint8 *)buf4, buf_len4);

            xSemaphoreGive(xSemaphore);
             vTaskDelay(30);
        }
    }
}
*/
void task4(void *pvPrameters)//canREG2(mcu-FPGA) task port3
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            lidar = receive[0]<<8;
            lidar |= receive[1];

                if(lidar <= 250 && flag_accelerate == 0) //already dsp choice(left, right)
                 {
                   vTaskSuspend(bldc_handle);
                   vTaskSuspend(servo_handle);
                   //vTaskSuspend(sci_handle);
                   vTaskSuspend(toggle_handle);

                  //canTransmit(canREG1, canMESSAGE_BOX1, (const uint8 *)&tx_data[0]);

                   etpwmREG1->CMPA = dsp_throttle;
                   etpwmREG2->CMPA = dsp_rudder;
                   //wait(2000000);
                   flag_lidar = 1;
                   dsp_flag = 1;

                 }
                if(lidar <= 250 &&  flag_accelerate == 1)//scenario 2
                {
                    vTaskSuspend(bldc_handle);
                    vTaskSuspend(servo_handle);
                    //vTaskSuspend(toggle_handle);
                    etpwmREG1->CMPA = 1500;
                    etpwmREG2->CMPA = 1250;
                    gioSetBit(gioPORTA, 2, 1);
                    gioSetBit(gioPORTA, 3, 1);
                    dsp_flag = 1;


                }
                 if(lidar > 250 && flag_lidar == 1 )
                 {
                    vTaskResume(bldc_handle);
                    vTaskResume(servo_handle);
                     //vTaskResume(sci_handle);
                     //wait(1000000);
                    flag_toggle = 1;
                    flag_lidar = 0;
                 }
                 if(lidar > 250 && flag_toggle == 1)
                 {
                    vTaskResume(toggle_handle);
                    flag_toggle = 0;
                    dsp_flag = 0;
                 }

            xSemaphoreGive(xSemaphore);
            vTaskDelay(30);
        }
    }
}

void task5(void *pvPrameters)//toggle switch output task
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            ecap_toggle();
            if(output>1900)
            {
                vTaskSuspend(bldc_handle);
                vTaskSuspend(servo_handle);
                //vTaskSuspend(sci_handle);
                //vTaskSuspend(dsp_can_handle);

                etpwmREG1->CMPA = 1560;
                etpwmREG2->CMPA = 1250;

                flag_output = 1;
                flag_accelerate = 0;
            }
            if(output<=1900 && flag_output == 1)
            {
                vTaskResume(bldc_handle);
                vTaskResume(servo_handle);
                flag_output = 0;
                flag_accelerate = 1;
            }

            xSemaphoreGive(xSemaphore);
             vTaskDelay(30);
        }
    }
}

void task6(void *pvPrameters)//led control task
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            ecap_channel5();
            ecap_channel6();
            ecap_channel8();
            if(etpwmREG1->CMPA == 1500)
            {
                gioSetBit(gioPORTA, 1 , 1);
            }else
            {
                gioSetBit(gioPORTA, 1, 0);
            }
            if(front_led > 1800)
            {
                gioSetBit(gioPORTA, 0, 1);
            }else
            {
                gioSetBit(gioPORTA, 0, 0);
            }
            if(blink_led >= 10 && blink_led < 1000)
            {
                gioSetBit(gioPORTA, 2, 1);
                flag_led = 0;
            }
            if(blink_led < 10)
            {
                gioSetBit(gioPORTA, 2, 0);
                gioSetBit(gioPORTA, 3, 0);
                flag_led = 0;
            }
            if(blink_led >= 1400 && blink_led <= 1600)
            {
                gioSetBit(gioPORTA, 2, 0);
                gioSetBit(gioPORTA, 3, 0);
                flag_led = 1;
            }
            if(blink_led >1900)
            {
                gioSetBit(gioPORTA, 3, 1);
                flag_led = 0;
            }
            if((emergency_light > 1900 ) && (flag_led == 1))//emergency light display
            {
                gioSetBit(gioPORTA, 2, 1);
                gioSetBit(gioPORTA, 3, 1);
                wait(10000000);
                gioSetBit(gioPORTA, 2, 0);
                gioSetBit(gioPORTA, 3, 0);
                wait(10000000);
                flag_led = 0;
            }
            xSemaphoreGive(xSemaphore);
             vTaskDelay(30);
        }
    }
}

void task7(void *pvPrameters)//velocity display task
{
    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t)0x01)==pdTRUE)
        {
            if((eqepREG2->QFLG & 0x800) == 0x800)
            {
                flag_eqep = (eqepREG2->QEPSTS & 0x20) >> 5;
            }
            deltaT = 0;

            deltapos = eqepReadPosnLatch(eqepREG2);


            if(deltapos < 0)
            {
                deltapos = eqepReadPosnLatch(eqepREG2);
                deltapos = -deltapos;
            }
            if(deltapos < 15)
            {
                deltapos = 0;
            }

            deltaT = eqepREG2->QUPRD; //per second


            eqepClearInterruptFlag(eqepREG2, QEINT_Uto);

            tx_data[3] = dsp_flag;//flag status
            tx_data[2] = deltapos % 100; //dsp setting data
            tx_data[1] = (deltapos / 100) % 100;
            tx_data[0] = deltapos / 10000;


            /*tx_data[0] = deltapos >> 24;
            tx_data[1] = deltapos >> 16;
            tx_data[2] = deltapos >> 8;
            tx_data[3] = deltapos;
            tx_data[4] = dsp_flag;

            tx_data[4] = deltapos >> 24;
            tx_data[5] = deltapos >> 16;
            tx_data[6] = deltapos >> 8;
            tx_data[7] = deltapos;*/

            xSemaphoreGive(xSemaphore);
            vTaskDelay(30);
        }
    }
}

void Init_all(void)//Initial function
{
    sciInit();
    gioInit();
    gioSetBit(gioPORTA, 0 , 0);
    gioSetBit(gioPORTA, 1,  0);
    gioSetBit(gioPORTA, 2,  0);
    gioSetBit(gioPORTA, 3,  0);
    ecapInit();
    ecapStartCounter(ecapREG1);
    ecapStartCounter(ecapREG2);
    ecapStartCounter(ecapREG3);
    ecapStartCounter(ecapREG4);
    ecapStartCounter(ecapREG5);
    ecapStartCounter(ecapREG6);
    ecapEnableCapture(ecapREG1);
    ecapEnableCapture(ecapREG2);
    ecapEnableCapture(ecapREG3);
    ecapEnableCapture(ecapREG4);
    ecapEnableCapture(ecapREG5);
    ecapEnableCapture(ecapREG6);
    etpwmInit();
    etpwmStartTBCLK();
    canInit();
    canEnableErrorNotification(canREG1);
    canEnableErrorNotification(canREG2);
    QEPInit();
    eqepSetUnitPeriod(eqepREG2, UnitPeriod);
    eqepEnableCounter(eqepREG2);
    eqepEnableUnitTimer(eqepREG2);
    eqepEnableCapture(eqepREG2);

    wait(100000);
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

void ecap_toggle(void)
{
    unsigned int cap[2];

    cap[0] = ecapGetCAP1(ecapREG1);
    cap[1] = ecapGetCAP2(ecapREG1);
    output = (cap[1] - cap[0])/VCLK3_FREQ;

}

void ecap_channel5(void)
{
    unsigned int cap[2];

    cap[0] = ecapGetCAP1(ecapREG4);
    cap[1] = ecapGetCAP2(ecapREG4);
    blink_led = (cap[1]-cap[0])/VCLK3_FREQ;
}

void ecap_channel6(void)
{
    unsigned int cap[2];

    cap[0] = ecapGetCAP1(ecapREG5);
    cap[1] = ecapGetCAP2(ecapREG5);
   emergency_light = (cap[1]-cap[0])/VCLK3_FREQ;
}

void ecap_channel8(void)
{
    unsigned int cap[2];

    cap[0] = ecapGetCAP1(ecapREG6);
    cap[1] = ecapGetCAP2(ecapREG6);
    front_led = (cap[1]-cap[0])/VCLK3_FREQ;
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
    if(node == canREG1)//dsp
    {
        while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2))
            ;
        canGetData(canREG1, canMESSAGE_BOX2, (uint8 *)&rx_data[0]);

        canTransmit(canREG1, canMESSAGE_BOX1, (const uint8 *)&tx_data[0]);


    }

    if(node == canREG2)//fpga
    {
        while(!canIsRxMessageArrived(canREG2, canMESSAGE_BOX2))
            ;
        canGetData(canREG2, canMESSAGE_BOX2, (uint8 *)&receive[0]);
    }
}


/* USER CODE END */
