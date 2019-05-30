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
#include "HL_gio.h"
#include "HL_esm.h"
#include "HL_sci.h"

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
extern void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void udpTask(void *pvParameters);
void sciTask(void *pvParameters);
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
extern void EMAC_LwIP_Main (uint8_t * emacAddress);

void sci_display_txt(sciBASE_t *sci, uint8 *txt, uint32 len);

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

SemaphoreHandle_t xSemaphore = NULL;

/* Define Task Handles */
xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;
xTaskHandle xTask3Handle;
xTaskHandle xTask4Handle;
xTaskHandle xTask5Handle;
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
uint8_t sciTest[] = {"SCI very well\r\n"};
//uint8 length = sizeof(sciTest);

uint16 pwm_CMPA;
char buf[32] = {0};
unsigned int buflen = 0;

#define SCI_DEBUG   0 // If this value set 1, sciREG1 prints Debug messages.

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

    //VCLK3_FREQ; // 37.500F
    etpwmInit();
    /* Set high end timer GIO port pin direction to all output */
    gioInit();
    gioSetDirection(gioPORTA, 0xFFFF);
    gioSetDirection(gioPORTB, 0xFFFF);

    EMAC_LwIP_Main(emacAddress);

    /* Create Task 1 */
    //    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL,  ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT), &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 2 */
    if (xTaskCreate(vTask2,"Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 3 */
#if 1
    if(xTaskCreate(pwmTask, "PWM", configMINIMAL_STACK_SIZE, NULL, 2, &xTask3Handle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 4 */
#if 1
    if(xTaskCreate(sciTask, "SCI", 2 * configMINIMAL_STACK_SIZE, NULL, 4, &xTask5Handle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 5 */
#if 1
    if(xTaskCreate(udpTask, "udp", 8 * configMINIMAL_STACK_SIZE, NULL, 3, &xTask4Handle) != pdTRUE)
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

void vTask2(void *pvParameters)
{
    for(;;)
    {
        taskENTER_CRITICAL();

#if 0
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
#endif
        /* Taggle GIOB[7] with timer tick */
            gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
#if 0
            xSemaphoreGive(xSemaphore);
        }
#endif

        taskEXIT_CRITICAL();
        //gioSetBit(hetPORT1, 18, gioGetBit(hetPORT1, 18) ^ 1);  //LED on HDK, bottom
        vTaskDelay(500);
    }
}

#if 1
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

void sciTask(void *pvParameters)
{
    /* SCI Baud Rate : 230400
     * sciREG1
     */
#if SCI_DEBUG
    sprintf(buf, "VCLK3_FREQ = %.2f\n\r\0", VCLK3_FREQ);
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);

    sprintf(buf, "SCI_Baud_Rate = 230400\n\r\0");
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);
#endif
    for(;;)
    {
        taskENTER_CRITICAL();

        sprintf(buf, "CMPA = %d\n\r\0", pwm_CMPA);
        buflen = strlen(buf);
        sci_display_txt(sciREG1, (uint8 *)buf, buflen);

        taskEXIT_CRITICAL();
        vTaskDelay(100);
    }
}

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
/* USER CODE END */
