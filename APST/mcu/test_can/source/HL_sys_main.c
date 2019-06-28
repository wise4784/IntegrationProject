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
#include "HL_gio.h"
#include "HL_can.h"
#include "HL_system.h"
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

uint8_t tx_data[DCNT] = {'A','B','C','1','2','3','a','\0'};
uint8_t rx_data[DCNT] = {0};

void wait(int delay)
{
    int i;
    for(i=0; i<delay; i++)
        ;
}

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    gioPORTB->DIR |= (1U << 6);
    printf("GIO Init Success\n");

    while(gioGetBit(gioPORTB,4))
    {
        wait(10000);
    }
    int i = 0;
    _enable_interrupt_();

    canInit();
    //printf("CAN Init Success\n");
    canEnableErrorNotification(canREG2);

    //canTransmit(canREG2, canMESSAGE_BOX1, tx_data);

    for(;;)
    {
        canTransmit(canREG2, canMESSAGE_BOX1, tx_data);

        //printf("tx_data Sending!!!\n");

        wait(3300000);

        gioToggleBit(gioPORTB, 6);

#if 1
        if(canIsRxMessageArrived(canREG2, canMESSAGE_BOX2))
        {
            canGetData(canREG2, canMESSAGE_BOX2, (uint8 *)&rx_data[0]);
            for(i = 0; i < DSIZE; i++)
                printf("rx_data[%d] = %x\n", i, rx_data[i]);
            wait(1000000);
        }
#endif
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void canMessageNotification(canBASE_t *node, uint32_t notification)
{
    if(node == canREG2)
    {
        while(!canIsRxMessageArrived(canREG2, canMESSAGE_BOX2))
            ;

        canGetData(canREG2, canMESSAGE_BOX2, rx_data);
        printf("rx_data : %x\n", *rx_data);
    }
}
/* USER CODE END */
