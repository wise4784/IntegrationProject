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
#include "HL_sci.h"
#include "HL_gio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

void wait(uint32 delay);

int i;

#define DCNT        7
uint8_t data[] = {'\r','\n'};
uint8_t data2[] = {"fw\r\n"};
// 플래쉬 저장
uint8_t tx_data[] = {0x02,0x09,0x02,0x18,0x07,0x00,0x00,0x01,0x00,0x00,0x00,0x22,0x03};
// 오일러 각 중 roll 데이터 주셈
uint8_t tx_data2[] = {0x02,0x09,0x02,0x3c,0x35,0x00,0x01,0x00,0x00,0x00,0x00,0x74,0x03};
// rs232 동기화 데이터 보내지 마라
uint8_t tx_data3[] = {0x02,0x09,0x02,0x18,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x2f,0x03};
uint8_t rx_data[13] = {0};
uint8_t rx_data2[13] = {0};
float roll;
int val;
char buf[] = {0};
uint8 buflen;
void convert(unsigned char *arr, unsigned char *num);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    gioPORTB->DIR |= (1U << 6);
    gioPORTB->DIR |= (0U << 4);
    printf("gio initializing..\n");
    sciInit();
    printf("sci Initializing!!\n");
#if 0
    while(!sciIsTxReady(sciREG3))
        ;
    sciSend(sciREG3, sizeof(tx_data3),tx_data3);
#endif
#if 0
    while(!sciIsRxReady(sciREG3))
        ;
    sciReceive(sciREG3, sizeof(rx_data2), rx_data2);
    printf("RS232 No data : ");
    for(i = 0; i < sizeof(rx_data2); i++)
        {
            printf("%2x",rx_data2[i]);
        }
    printf("\n");
#endif

#if 0
        while(!sciIsTxReady(sciREG3))
            ;
        sciSend(sciREG3, sizeof(tx_data),tx_data);
#endif
#if 0
        while(!sciIsRxReady(sciREG3))
            ;
        sciReceive(sciREG3, sizeof(rx_data), rx_data);
        printf("Flash save : ");
        for(i = 0; i < sizeof(rx_data); i++)
        {
            printf("%2x",rx_data[i]);
        }
        printf("\n");
#endif
    for(;;)
    {
        //gioToggleBit(gioPORTB, 6);

#if 0
        while(!sciIsTxReady(sciREG3))
                ;
        sciSend(sciREG3, sizeof(data),data);
        wait(1000);

        while(!sciIsTxReady(sciREG3))
            ;
        sciSend(sciREG3, sizeof(data2),data2);
        wait(1000);
#endif
#if 1
        while(!sciIsTxReady(sciREG3))
            ;
        sciSend(sciREG3, sizeof(tx_data2),tx_data2);
#endif
        while(!sciIsRxReady(sciREG3))
            ;
        sciReceive(sciREG3, sizeof(rx_data2), rx_data2);
#if 0
        while(!sciIsTxReady(sciREG1))
                    ;
        sciSend(sciREG1, sizeof(rx_data2), rx_data2);
        while(!sciIsTxReady(sciREG1))
                    ;
        sciSend(sciREG1, sizeof(data),data);
#endif
#if 0
        printf("roll data : ");
        for(i = 0; i < sizeof(rx_data2); i++)
        {
            printf("%2x",rx_data2[i]);
        }
        printf("\n");
#endif
        /*
        roll = rx_data2[10] << 24
                | rx_data2[9] << 16
                | rx_data2[8] << 8
                | rx_data2[7];
                */
        convert(rx_data2, &roll);
        //printf("data : %f\n", roll);
        sprintf(buf,"roll = %f\r\n", roll);
        buflen = strlen(buf);

        while(!sciIsTxReady(sciREG1))
                ;
        sciSend(sciREG1, buflen, buf);

#if 0
        while(!sciIsRxReady(sciREG3))
            ;

        sciReceive(sciREG3, sizeof(rx_data), rx_data);
#if 0
        printf("rx_data  : ");
        for(i = 0; i < sizeof(rx_data); i++)
        {
            printf("%2x",rx_data[i]);
        }
        printf("\n");
#endif
#endif

        wait(1000);
    }
/* USER CODE END */

    return 0;
}

/* USER CODE BEGIN (4) */
void convert(unsigned char *arr, unsigned char *num)
{
    int i;
    for(i = 0; i < 4; i++)
        num[i]  = arr[10 - i];
}

void wait(uint32 delay)
{
    int j;
    for(j = 0; j < delay; j++)
        ;
}
/* USER CODE END */
