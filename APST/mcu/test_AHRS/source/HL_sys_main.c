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
#include "HL_can.h"
#include "HL_gio.h"
#include <stdio.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
int i;

#define DCNT        8

uint8_t tx_data[DCNT] = {0x18, 0x16, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00};
uint8_t tx_data2[DCNT] = {0x18, 0x18, 0x00, 0x00, 0xE8, 0x03, 0x00, 0x00};
#if 1
uint8_t roll_data[DCNT] = {0x3C, 0x35, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
#else
uint8_t roll_data[DCNT] = {0x01, 0x00, 0x35, 0x3C, 0x00, 0x00, 0x00, 0x00};
#endif
uint8_t pich_data[DCNT] = {0x3C, 0x35, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00};

uint8_t rx_data[DCNT] = {0};

void wait(uint32 delay);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    gioPORTB->DIR |= (1U << 6);
    gioPORTB->DIR |= (0U << 4);

    printf("gio initializing..\n");

    _enable_interrupt_();

    canInit();
    printf("can initializing..\n");
    canEnableErrorNotification(canREG2);

    for(;;)
    {
        gioToggleBit(gioPORTB, 6);

        canTransmit(canREG2, canMESSAGE_BOX1, roll_data);
#if 0
        if(canIsRxMessageArrived(canREG2, canMESSAGE_BOX2))
        {
            canGetData(canREG2, canMESSAGE_BOX2, rx_data);

            printf("rx_data : ");
            for(i = 0; i < 8; i++)
            {
                printf("%x\t", rx_data[i]);
            }
            printf("\n");
        }
#endif
        wait(30000000);
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void wait(uint32 delay)
{
    int j;
    for(j = 0; j < delay; j++)
        ;
}

void canMessageNotification(canBASE_t *node, uint32_t notification)
{
    if(node == canREG2)
    {
        while(!canIsRxMessageArrived(canREG2, canMESSAGE_BOX2))
            ;

        canGetData(canREG2, canMESSAGE_BOX2, rx_data);

        printf("rx_data : ");
        for(i = 0; i < 8; i++)
        {
            printf("%x\t", rx_data[i]);
        }
        printf("\n");
    }
}
/* USER CODE END */
