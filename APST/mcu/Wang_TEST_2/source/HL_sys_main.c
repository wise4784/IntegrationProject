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
#include "HL_het.h"
#include "HL_gio.h"
#include "HL_esm.h"
#include "HL_sci.h"
#include "HL_etpwm.h"

#if 0
#include "ipv4/lwip/ip_addr.h"
#include <src/include/lwip/api.h>
#endif

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void vTask1(void *pvParameters);
extern void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void vTask4(void *pvParameters);
void vTask5(void *pvParameters);
void vTask6(void *pvParameters);
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
void sciDisplayText(sciBASE_t *sci, uint8_t *text,uint32_t length);

//extern void EMAC_LwIP_Main (uint8_t * emacAddress);

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
uint8 flag = 0;
uint8_t sciTest[] = {"SCI very well\r\n"};
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

    xSemaphore = xSemaphoreCreateBinary();

    if(xSemaphore == NULL)
    {
        return -1;
    }
    /* Set high end timer GIO port hetPort pin direction to all output */
    sciInit();
    gioInit();
    gioSetDirection(gioPORTA, 0xFFFF);
    gioSetDirection(gioPORTB, 0xFFFF);

    /* Set high end timer GIO port hetPort pin direction to all output */
    gioSetDirection(hetPORT1, 0xFFFFFFFF); //HDK uses NHET for LEDs

    //EMAC_LwIP_Main(emacAddress);

    /* Create Task 1 */
    //    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL,  ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT), &xTask1Handle) != pdTRUE)
    {
        while(1);
    }
    /* Create Task 2 */
    if (xTaskCreate(vTask2,"Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle) != pdTRUE)
    {
        while(1);
    }
#if 1
    if(xTaskCreate(vTask3, "PWM", configMINIMAL_STACK_SIZE, NULL, 2, &xTask3Handle) != pdTRUE)
    {
        while(1);
    }
#endif
#if 1
    if(xTaskCreate(vTask4, "HET", configMINIMAL_STACK_SIZE, NULL, 3, &xTask4Handle) != pdTRUE)
    {
        while(1);
    }
#endif
#if 1
    if(xTaskCreate(vTask5, "SCI", configMINIMAL_STACK_SIZE, NULL, 4, &xTask5Handle) != pdTRUE)
    {
        while(1);
    }
#endif
#if 0
    if(xTaskCreate(vTask6, "udp_send", configMINIMAL_STACK_SIZE, NULL, 5, &xTask6Handle) != pdTRUE)
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
    EMAC_LwIP_Main(emacAddress);
    for(;;)
    {
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
            /* Taggle GIOB[6] with timer tick */
            gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
            xSemaphoreGive(xSemaphore);
        }

        /* Taggle HET[1] with timer tick */
        //gioSetBit(hetPORT1, 17, gioGetBit(hetPORT1, 17) ^ 1);  //LED on HDK, top left
        vTaskDelay(300);
    }
}

void vTask2(void *pvParameters)
{
    for(;;)
    {
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
        /* Taggle GIOB[7] with timer tick */
            gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
            xSemaphoreGive(xSemaphore);
        }
        //gioSetBit(hetPORT1, 18, gioGetBit(hetPORT1, 18) ^ 1);  //LED on HDK, bottom
        vTaskDelay(500);
    }
}

void vTask3(void *pvParameters)
{
    /*
     * etpwmInit 시 etpwmStartTBCLK() 하면 태스크 멈춤.
     * Semaphore를 하던가 테스크 시작 전에 하거나 해야할듯.
     * 현재는 주석처리 해놨음.
     * HALCogen generate 마다 주석처리 해줘야함. 아니면 Init을 주석처리하거나
     * HL_sys_link.cmd도 수정해줘야함. Memory맵 안에 User코드와 겹치는 위쪽 3줄 지워줘야함
     */
    etpwmInit();
    uint8 flag = 0;
    for(;;)
    {
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
#if 0
            // 세마포어 안에서 Start해도 태스크 멈춤. (Race condition...)
            if(flag == 0)
            {
                etpwmStartTBCLK();
                flag = 1;
            }
#endif
            gioSetBit(gioPORTB, 0, gioGetBit(gioPORTB, 0) ^ 1);
            xSemaphoreGive(xSemaphore);
        }
        vTaskDelay(150);
    }
}

void vTask4(void *pvParameters)
{
    for(;;)
    {
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
            gioSetBit(hetPORT1, 4, gioGetBit(hetPORT1, 4) ^ 1);
            xSemaphoreGive(xSemaphore);
        }
        vTaskDelay(400);
    }
}

void vTask5(void *pvParameters)
{
    /* SCI Baud Rate : 230400
     * sciREG1
     */
    uint8 length = sizeof(sciTest);
    int i = 0;
    for(;;)
    {
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
            if(length > 0)
            {
                if((sciREG1->FLR & 0x4) != 4)
                {
                    sciSendByte(sciREG1, sciTest[i]);
                    i++;
                    length--;
                }
            }
#if 1
            else
            {
                gioSetBit(gioPORTA, 0, gioGetBit(gioPORTA, 0) ^ 1);
            }
#endif
            xSemaphoreGive(xSemaphore);
        }
        vTaskDelay(10);
    }
}
#if 0
void vTask6(void *pvParameters)
{
    struct netconn *conn;
    struct netbuf *buf;
    struct ip_addr addr;
    char *data;
    char text[] = "A static text";
    int i;

    conn = netconn_new(NETCONN_UDP);

    for(;;)
    {
        vTaskDelay(10);
    }
}
#endif
/* USER CODE END */
