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
#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_adc.h"
#include "HL_system.h"
#include "stdio.h"
#include <string.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define SIZE1       32
char TXT2[SIZE1] = {0,};
#define vCoeff  0.24176              //cap_bank voltage-coefficent
#define dSize   10                   // data 10


float getAvg(int *buf, int value);  // MoveAvgFilter
void sci_display(sciBASE_t * sci, uint8 * txt, uint32 len);
void wait(uint32);

adcData_t adc_data[2];
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    float cVolt= 0;
    int dBuf[dSize] = {0};
    char TXT1[9] = {'\n', '\r', 'c', 'V', 'o', 't', ' ', '=', ' '};
    uint16 value;
    //etpwmInit();
    gioInit();
    adcInit();
    sciInit();

    //etpwmStartTBCLK();

    adcStartConversion(adcREG1, adcGROUP1);

    for(;;)
    {
        while((adcIsConversionComplete(adcREG1, adcGROUP1))==0)
                    ;

        adcGetData(adcREG1, adcGROUP1, &adc_data[0]);

        value = adc_data[0].value;

        if(gioGetBit(gioPORTB, 5) == 0)
        {
            gioSetBit(gioPORTA, 7, 1);
            //wait(0x3000);
            //gioSetBit(gioPORTA, 7, 0);
        }
        cVolt = getAvg(dBuf, value);

        if(cVolt < 100)
        {
            gioSetBit(gioPORTA, 7, 0);
        }
        //memset(&TXT2[0],0,32);
        memcpy(&TXT2[0],&TXT1[0],9);
        sprintf(&TXT2[9], "%f", cVolt);

        sci_display(sciREG1, (uint8 *)&TXT2[0], SIZE1);
        wait(0x10);
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

float getAvg(int *buf, int value)
{
    int i, total;

    for(i = 0; i < dSize-1; i++)
     {
            buf[i] = buf[i+1];
     }

     buf[dSize-1] = value;

     for(i = 0; i < dSize; i++)
     {
         total += buf[i];

     }

    return (((total/dSize) * vCoeff)+24.703);           //0.2298x+14.703
}

void wait(uint32 delay)
{
    int i;

    for(i=0; i<delay; i++)
        ;
}

void sci_display(sciBASE_t * sci, uint8 * txt, uint32 len)
{
    int cnt =0;
    while(txt[cnt])
    {
        while((sciREG1->FLR & 0x4) == 4)
                ;
        sciSendByte(sciREG1,txt[cnt]);
        cnt++;
    }
}
/* USER CODE END */
