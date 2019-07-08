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
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define UnitPeriod 1000000

char buf[128] = {0};
uint8 receive_data[5]={0};
unsigned int buf_len;
float value = 0.0;
int flag = 0;

float set_p = 0;

void wait(int);
void sci_display(sciBASE_t * sci, uint8 * text, uint32 len);
void catch_command(sciBASE_t * sci);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */

    uint32 deltapos = 0U;
    uint32 deltaT = 0U;
    float velocity = 0U;
    float t_angle = 0U;
    float t_time = 0U;

    sciInit();
    wait(1000);

    //sciREG1 : UART
    sprintf(buf, "SCI Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
    wait(100);

    etpwmInit();
    wait(1000);

    sprintf(buf, "PWM Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
    wait(100);

    QEPInit();
    wait(1000);

    //Unit Period Setting (QUPRD 설정)
    eqepSetUnitPeriod(eqepREG2, UnitPeriod);
    //Counter 활성화
    eqepEnableCounter(eqepREG2);
    //Unit Timer 활성화
    eqepEnableUnitTimer(eqepREG2);
    //capture 활성
    eqepEnableCapture(eqepREG2);

    sprintf(buf, "QEP Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
    wait(100);

    for(;;)
    {
        //위에서 QFLG:UTO비트(12번)가 set 되었는지 확인
       if((eqepREG2->QFLG & 0x800U) == 0x800U)
        {
          // QEPSTS의 0x20은 direction을 나타냄
           //현재의 방향을 shift해서 flag에 저장, 1이면 정방향, 0이면 역방향
          flag = (eqepREG2->QEPSTS & 0x20) >> 5;
            //0010 0000

          sprintf(buf, "direction : %d\n\r\0", flag);
          buf_len = strlen(buf);
          sci_display(sciREG1, (uint8 *)buf, buf_len);
          wait(100);

          //deltaT : QUPRD값을 저장하는 변수
         deltaT = 0;

         //flag가 1이면 정방
         if(flag == 1)
          {
                 //deltapos에 QPOSLAT 값을 읽음
                deltapos = eqepReadPosnLatch(eqepREG2);

                sprintf(buf, "delta : %d\n\r\0", deltapos);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
                wait(100);

            }
            else
            {//0이면 역방향
                deltapos = eqepReadPosnLatch(eqepREG2);
                deltapos = ~deltapos + 1;

                sprintf(buf, "delta : %d\n\r\0", deltapos);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
                wait(100);

            }

            deltaT = eqepREG2->QUPRD;

            sprintf(buf, "QUPRD : %d\n\r\0", deltaT);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            //각속도 계산 = 각도 / 시간
            // 시간 = deltaT(QUPRD) * (VCLK3 주기)
            //                     VCLK3 = 10MHz이므로 주기는 1/10000000
            // 각도 = (360/500) * deltapos * 0.25
            //                              0.25를 곱하는 이유는 우리가 사용하는 QCLK를 QUADRATURE_COUNT로 설정했기 때문
            t_time = (float)deltaT * 0.0000001;
            t_angle = (360.0 / 500.0) * (float)deltapos * 0.25;
            velocity = t_angle / t_time;

            sprintf(buf, "velocity : %f\n\r\0", velocity);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            //위의 velocity는 1초당 이동 각도
            //rpm은 1분당 rotation이므로 60 / 360 을 곱해줌
            sprintf(buf, "rpm : %f\n\n\r\0", velocity / 6.0);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            //interrupt flag를 clear함
            eqepClearInterruptFlag(eqepREG2, QEINT_Uto);
        }
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
/* USER CODE END */
