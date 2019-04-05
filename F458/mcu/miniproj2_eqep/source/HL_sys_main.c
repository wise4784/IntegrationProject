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

#include "HL_etpwm.h"
#include "HL_eqep.h"
#include "HL_sci.h"
#include <stdio.h>
#include <string.h>

// sci1 , eqep2

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

char buf[128] = {0};
uint8 receive_data[5]={0};
unsigned int buf_len;

uint32 value = 0;
int flag = 0; // about direction

#define UNIT_POSITION_X 0.23077 // 360/1560 (1560 is QCLK that is hardware spec)

void wait(int);
void sci_display(sciBASE_t * sci, uint8 * text, uint32 len);
void catch_command(void);

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    uint32 deltapos = 0U;
    uint32 deltaT = 0U;
    float velocity = 0U;

    sciInit();
    wait(1000);

    sprintf(buf, "SCI Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    etpwmInit();
    wait(1000);

    sprintf(buf, "PWM Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    QEPInit();
    wait(1000);

    eqepSetUnitPeriod(eqepREG2, 10000000); // make 1s (clock is 10MHz from VCLK3)
    eqepEnableCounter(eqepREG2);
    eqepEnableUnitTimer(eqepREG2);
    eqepEnableCapture(eqepREG2);

    sprintf(buf, "QEP Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    for(;;)
    {
        if((eqepREG2->QFLG & 0x800U) == 0x800U)
        {
            flag = (eqepREG2->QEPSTS & 0x20) >> 5;
            //0010 0000

            sprintf(buf, "flag : %d\n\r\0", flag);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);

            deltaT = 0;

            if(flag == 1)
            {
                deltapos = eqepReadPosnLatch(eqepREG2);

                sprintf(buf, "delta : %d\n\r\0", deltapos);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
            }
            else
            {
                deltapos = eqepReadPosnLatch(eqepREG2);
                deltapos = ~deltapos + 1;

                sprintf(buf, "delta : %d\n\r\0", deltapos);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
            }

            deltaT = eqepREG2->QUPRD;

            sprintf(buf, "QUPRD : %d\n\r\0", deltaT);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);

            velocity = (360.0 * (float)deltapos )/( 1560.0 * 0.0000001 * (float)eqepREG2->QUPRD);
/*
            velocity = delta X / T (This velocity is angular velocity per sec)
            delat X = (360 * deltapos) / 1560
            T = 1 sec ((1/10MHz)* 10000000)
*/
            sprintf(buf, "velocity : %f\n\r\0", velocity);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);

            sprintf(buf, "rpm : %f\n\n\r\0", velocity / 6.0);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);

            eqepClearInterruptFlag(eqepREG2, QEINT_Uto);

        }

        /*

        catch_command();

        value = (receive_data[0]-48)*1000

                + (receive_data[1]-48)*100

                + (receive_data[2]-48)*10

                + (receive_data[3]-48);

        etpwmREG1->CMPA = 1.25 * value;

        sprintf(buf, "value = %d\n\r\0", value);

        buf_len = strlen(buf);

        sci_display(sciREG1, (uint8 *)buf, buf_len);

        */

    }

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

void sci_display(sciBASE_t * sci, uint8 * text, uint32 len)
{

    while(len--)
    {

        while((sci->FLR & 0x4) == 4)   //SCI receive in idle state
            ;                           //0 : ready to receive

        sciSendByte(sci, *text++);     //1 : not receive any data.
    }
}

void wait(int delay)
{

    int i;

    for(i=0; i<delay; i++)
        ;
}

void catch_command(void)
{
    int i;

    while((sciREG1->FLR & 0x4) == 4)
        ;

    for(i=0; i<5; i++)
    {
        receive_data[i] = sciReceiveByte(sciREG1);
    }
}

/* USER CODE END */
