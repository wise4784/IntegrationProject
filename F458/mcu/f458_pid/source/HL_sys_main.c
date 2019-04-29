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
/*
 * etPWM : O, eQEP : O, UART : O, eCAP : X, RTOS : X, HET : X
 * etPWM : PWM1
 * eQEP  : eQEP2
 * UART  : sci3
 * GCM   : VLCK3 divide 14 -> VLCK3 = 10.000MHZ
*/
#include "HL_etpwm.h"
#include "HL_eqep.h"
#include "HL_sci.h"
#include "HL_system.h"
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

char buf[128] = {0};
unsigned int buf_len;
uint32  k[6] = {0};
// k[0] -> 1 : kp, 2 : ki, 3 : kd
// k[1]*10, k[2]*1, k[3]*0.1, k[4]*0.01, k[5]*0.001
uint32  e_set = 0U;
uint32  e_input = 0U;
uint32  e_old = 0U;
uint32  setpos = 0U;
uint16  pwmout = 0U;
float   kp = 0U;
float   ki = 0U;
float   kd = 0U;
float   Perror  = 0U;
float   Ierror  = 0U;
float   Derror  = 0U;
float   PID_value = 0U;
float   unit_time = 0U;

uint32 deltapos = 0U;
uint32 deltaT = 0U;
float Ang_velocity = 0U;
float velocity = 0U;
float rpm = 0U;

//float sAng_velocity = 0U;
//float svelocity = 0U;
//float srpm = 0U;

int flag = 0; // about direction to use eQEP

#define UNIT_POSITION_X 0.23077 // 360/1560 (1560 is QCLK that is hardware spec)

void wait(int);
void sci_display(sciBASE_t * sci, uint8 * text, uint32 len);
void catch_command(void);

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    sciInit();

    sprintf(buf, "SCI Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    etpwmInit();


    sprintf(buf, "PWM Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    QEPInit();


    eqepSetUnitPeriod(eqepREG2, 1000000); // make 0.1sec (clock is 10MHz from VCLK3)
    eqepEnableCounter(eqepREG2);
    eqepEnableUnitTimer(eqepREG2);
    eqepEnableCapture(eqepREG2);

    sprintf(buf, "QEP Init Success!!!\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    etpwmREG1->CMPA = 0;

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

                sprintf(buf, "deltapos : %d\n\r\0", deltapos);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
            }
            else
            {
                deltapos = eqepReadPosnLatch(eqepREG2);
                deltapos = ~deltapos + 1; // 역방향일때 레지스터가 FF부터 내려오므로
                                // 정방향일때와 같은 변위를 같게 하기위해 ~을 해주고 +1을 더함
                            // eqep 신호 값 즉 -> 파형의 갯수 값
                sprintf(buf, "deltapos : %d\n\r\0", deltapos);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
            }

            //deltaT = eqepREG2->QUPRD; // 이 시간(주기)은 사용자가 설정해주는 값
/*
            sprintf(buf, "QUPRD : %d\n\r\0", deltaT);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
*/
            //encoder quality = 500 -> 500 * 4 = 2000
            //velocity = delta X / T (This velocity is angular velocity per sec)
            //delat X = (360 * deltapos) / 1560
            //T = 0.1 sec (1/10MHz)

            Ang_velocity = (360.0 * (float)deltapos )/( 2000 * 0.0000001 * (float)eqepREG2->QUPRD);
            sprintf(buf, "Ang_velocity : %f\n\r\0", Ang_velocity);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            // rpm
            rpm = Ang_velocity / 6.0;
            sprintf(buf, "rpm : %f\n\n\r\0", rpm);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            // velocity
            velocity =  (3.141 * 10.0 * rpm) / (30.0);
            sprintf(buf, "velocity : %f\n\n\r\0", velocity);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            //**********simple pid start************
           /*
            // 속도 -> 펄스 개수 변환 코드
            svelocity  = 2135.2; //   2135.2 cm/m -> 1.28km/h // rpm 64
            srpm = svelocity * (30.0) / (3.141 * 10.0);
            sAng_velocity = rpm * 6.0;
            setpos = (sAng_velocity * ( 2000 * 0.0000001 * (float)eqepREG2->QUPRD)) / 360.0;
            */
            kp = 11;
            ki = 2;
            kd = 0;
            e_set = 113; //사용자 설정 속도 값(0.1당 펄스수)
            e_input = deltapos;
            Perror = e_set - e_input;
            Ierror = Ierror + ((e_set - e_input)*0.1); //  0.1 를 0.1sec가 되도록 바꾼다.
            Derror = (e_input - e_old) / 0.1;
            // k 값들 asci2값 참조
            //if(Perror < 0)
            //  Perror = -Perror;

            PID_value = (kp * Perror) + (ki * Ierror) + (kd * Derror);

            // 마지막 0.1을 0.1sec가 되도록바꾼다.

            e_old = (e_set - e_input);

             //문제점 PID_value 는 float인데 CMPA는 uint형이므로 소수점이 들어가면 안됨
            pwmout = PID_value; // 여기서 소수점 짤림, 정수값만 남음
            sprintf(buf, "pwmout : %d\n\n\r\0", pwmout);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            wait(100);

            // uint16 CMPA
            if(pwmout > 1800){
                etpwmREG1->CMPA = 1800;
            }
            else if(pwmout < 1500){
                etpwmREG1->CMPA = 1550;
            }
            else{
                etpwmREG1->CMPA = pwmout;
            }
            //pid end


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

void catch_command(void)
{
    int i;

    while((sciREG3->FLR & 0x4) == 4)
        ;

    for(i=0; i<6; i++)
    {
        k[i] = sciReceiveByte(sciREG3);
    }
}

/* USER CODE END */
