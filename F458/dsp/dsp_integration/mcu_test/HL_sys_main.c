/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2016 Texas Instruments Incorporated - www.ti.com  
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
#include <math.h>

#include "HL_can.h"
#include "HL_sci.h"
#include "HL_esm.h"
#include "HL_system.h"
#include "HL_sys_core.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define DCNT        8
#define DSIZE       8

uint32 cnt = 0;
uint32 err = 0;
uint32 tx_done = 0;

uint8_t tx_data[DCNT] = { 0, 8, 10, 0,};
uint8_t tx_data2[DCNT] = {0, 35, 00, 1,};
uint8_t tx_data3[DCNT] = {0, 43, 00, 0,};

uint8_t rx_data[DCNT] = {0};

int can_con_flag;

void wait(uint32);
void sci_display(sciBASE_t *sci, uint8 *text, uint32 len);
int num=0;
int i = 0;
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    char buf[256];
    unsigned int buf_len;    int data;

    sciInit();

    sprintf(buf, "SCI Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    _enable_interrupt_();           // FIQ interrupt를 사용하려면 enable_interrupt()를 사용해야한다. _enable_IRQ_interrupt_()는 안된다.
    canInit();

    //printf("CAN Init Success\n");

    sprintf(buf, "CAN Init Success!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    canEnableErrorNotification(canREG1);

    for(;;)
        ;
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
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
        while((sciREG1->FLR & 0x4) == 4)
            ;
        sciSendByte(sciREG1, *text++);
    }
}

void canMessageNotification(canBASE_t *node, uint32_t notification)
{
    if(node == canREG1)
    {
        while(!canIsRxMessageArrived(canREG1, canMESSAGE_BOX2))
            ;

        canGetData(canREG1, canMESSAGE_BOX2, rx_data);
        num++;

        printf("i = %d, line2_flag = %d\n", i, rx_data[5]);

        wait(100);

        switch(i%3)
        {
        case 0:
            //printf("canTransmit 1\n");
            canTransmit(canREG1, canMESSAGE_BOX1, tx_data);
            break;
        case 1:
            //printf("canTransmit 2\n");
            canTransmit(canREG1, canMESSAGE_BOX1, tx_data2);
            break;
        case 2:
            //printf("canTransmit 3\n");
            canTransmit(canREG1, canMESSAGE_BOX1, tx_data3);
            break;
        default:
            //printf("canTransmit 1\n");
            canTransmit(canREG1, canMESSAGE_BOX1, tx_data);
            break;
        }
        i++;
    }
}
/* USER CODE END */
