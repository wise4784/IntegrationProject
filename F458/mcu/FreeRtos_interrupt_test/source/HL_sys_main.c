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
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "os_task.h"
#include "os_queue.h"
#include "os_semphr.h"

#include "HL_sci.h"
#include "HL_can.h"
#include "HL_ecap.h"
#include "HL_esm.h"
#include "HL_system.h"

#define UART    sciREG1
#define DCNT    8
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

char buf[128];
unsigned int buf_len;

int throttle;
uint32 cap[2];
uint8_t tx_data[DCNT] = {3, 3, 3, 3, 7, 7, 7, 7};
uint8_t rx_data[DCNT] = {0};

/*void wait(uint32 delay)
{
    int i;
    for(i=0; i<delay; i++)
        ;
}*/
void sci_display(sciBASE_t *,uint8 *, uint32);

void task0_func(void *);
void task1_func(void *);

SemaphoreHandle_t xSemaphore;
TaskHandle_t ecap_handle;
TaskHandle_t can_handle;



/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    sciInit();
    sprintf(buf, "SCI Init Success\n\r\0");
    buf_len = strlen(buf);
    sci_display(UART, (uint8 *)buf, buf_len);


    ecapInit();
    sprintf(buf, "ECAP Init Success\n\r\0");
    buf_len = strlen(buf);
    sci_display(UART, (uint8 *)buf, buf_len);

    ecapStartCounter(ecapREG1);
    ecapEnableCapture(ecapREG1);

    canInit();
    sprintf(buf, "can Init Success\n\r\0");
    buf_len = strlen(buf);
    sci_display(UART, (uint8 *)buf, buf_len);



    xSemaphore = xSemaphoreCreateBinary(); //�꽭留덊룷�뼱 �깮�꽦�븿�닔(�솕�옣�떎 移몃쭑�씠 �깮�꽦)

    if(xTaskCreate(task0_func,"ecap_tx", configMINIMAL_STACK_SIZE*2, NULL, 5, ecap_handle)!=pdPASS) //task0 �깮�꽦
    {
        for(;;)
            ;
    }

   if(xTaskCreate(task1_func, "can_rx", configMINIMAL_STACK_SIZE*2, NULL,1, can_handle)!=pdPASS) //task1 �깮�꽦
    {
         for(;;)
             ;
    }

    vTaskStartScheduler(); //�뒪耳�伊대윭媛� task�뱾�쓣 愿�由� �닔�뻾

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void task0_func(void *pvParameters)
{


    for(;;)
    {
        if(xSemaphoreTake(xSemaphore, (TickType_t )0x01)==pdTRUE) // �떎瑜� task媛� �젒洹쇳븯吏� 留먮씪怨� 臾� �떕�뒗 寃�
        {
                cap[0] = ecapGetCAP1(ecapREG1);
                cap[1] = ecapGetCAP2(ecapREG1);

                throttle = (cap[1]-cap[0])/VCLK3_FREQ; //channel 3
                sprintf(buf, "ch3 = %d ms\n\r", throttle);
                buf_len = strlen(buf);
                sci_display(UART, (uint8 *)buf, buf_len);

                if(throttle > 1700)
                {
                    //wait(10000000);
                    canTransmit(canREG1, canMESSAGE_BOX1, (const uint8 *)&tx_data);
                 }
                else
                {
                    canTransmit(canREG1, canMESSAGE_BOX1, (const uint8 *)&rx_data);
                }

            }
        xSemaphoreGive(xSemaphore);
        vTaskDelay(10);
    }

}
void task1_func(void *pvParameters)
{
    for(;;)
        ;
}

void canMessageNotification(canBASE_t *node, uint32_t notification)
{
    if(node == canREG1){


    while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2))
         ;
     canGetData(canREG1, canMESSAGE_BOX2, (uint8 *)&rx_data[0]);
         printf("rx_data: %x\n", rx_data[0]);
}
}

void sci_display(sciBASE_t *sci, uint8 *text, uint32 len )
{
    while(len--)
    {
        while((UART->FLR & 0x4) == 4)
            ;
        sciSendByte(UART, *text++);
    }
}



/* USER CODE END */
