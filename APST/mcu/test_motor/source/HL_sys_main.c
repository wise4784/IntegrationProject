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
#include "HL_etpwm.h"
#include "HL_eqep.h"
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
void sci_display_data(sciBASE_t *sci, uint8 *data, uint32 len);
void sci_display_txt(sciBASE_t *sci, uint8 *txt, uint32 len);

// 딜레이 함수
void wait(uint32);

// 브레이크 동작확인 함수 (Active High)
void check_break(void);

// pwm 클럭은 1Mhz
// 모터 pwm 변경 함수
// 주파수 = period * 0.000001
void chg_pwm(etpwmBASE_t *etpwm, uint16 period, int duty);

// 모터를 일정한 속도로 회전
void const_velocity(void);

/*  Pin 설정
 *  GPIOA_3 -> BREAK
 *  GPIOA_5 -> Direction
 *  GPIOB_4 -> 유저 버튼
 *  GPIOB_6 -> 유저 LED
 *  etPWM4_A -> PWM (J10_26)
 *  eQEP1_A -> Hall_A
 *  eQEP1_B -> Hall_B
 */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    sciInit();
    gioInit();
    gioSetBit(gioPORTA, 5, 1);
    gioSetBit(gioPORTA, 3, 0);
    //주기 1ms 주파수 1khz
    etpwmInit();
//    etpwmStopTBCLK();
    QEPInit();
    eqepSetUnitPeriod(eqepREG1, 375000);

//    chg_pwm(etpwmREG4, 37500, 1);

    // 유저 버튼 누를때까지 대기
    while(gioGetBit(gioPORTB,4) == 1)
        ;

    //eqepSetCapturePeriod(eqepREG1, 37500);
//    etpwmStartTBCLK();

//    chg_pwm(etpwmREG4, 37500, 10);
//    etpwmREG4->CMPA = 37500;

    eqepEnableCounter(eqepREG1);
    eqepEnableUnitTimer(eqepREG1);

    uint32 cnt = 0;
    uint32 icnt = 0;
    uint32 duty = 0;
    float velocity = 0;

    // PPR이 맞는 지 확인하기위해 MAX 값 설정
//    eqepSetMaxPosnCount(eqepREG1, 1824);

    char buf[128] = {0};
    unsigned int buflen = 0;


    for(;;)
    {
 //       cnt = eqepReadPosnCount(eqepREG1);
#if 1
        if((eqepREG1->QFLG & 0x800) == 0x800)
        {
//            gioSetBit(gioPORTA,5,1);
//            cnt = eqepReadPosnCount(eqepREG1);
//            eqepREG1->QPOSCNT  =  0x00000000U;
            icnt = eqepReadPosnLatch(eqepREG1); // 정해놓은 시간동안 들어온 CNT 갯수
            velocity = ((float)eqepREG1->QPOSLAT * 0.75 / 0.01) / 6;
/*
            cnt = eqepReadPosnCount(eqepREG1); Time Out 발생 시 확실하게 초기화됌.
 *
 *          sprintf(buf, "PCNT = %d\n\r\0", cnt);
 *          buflen = strlen(buf);
 *          sci_display_txt(sciREG1, (uint8 *)buf, buflen);
*/
            sprintf(buf, "POSCNT = %d\n\r\0", icnt);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

            sprintf(buf, "Velocity = %.3f\n\r\0", velocity);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

//          gioToggleBit(gioPORTB, 6);
//          gioToggleBit(gioPORTA, 5);
            // Flag가 자동 초기화가 안됌.
            eqepClearInterruptFlag(eqepREG1, QEINT_Uto);
        }
#endif
#if 0
        if(gioGetBit(gioPORTB,4) == 0)
        {
            chg_pwm(etpwmREG4, 37500, 5);

            duty = 100 * (etpwmREG4->CMPA) / 37500;

            sprintf(buf, "Duty = %d\n\r\0", duty);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);
            wait(500000);
        }
#endif

        // 한 바퀴 펄스 갯수 테스트
       // POSCNT 1당 0.75도
#if 0
        if(cnt > 480)
        {
//            gioSetBit(gioPORTA, 3, 1);
            sprintf(buf, "Over = %d\n\r\0", a);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

//            while(gioGetBit(gioPORTB,4) == 1)
//                ;

//            gioSetBit(gioPORTA, 3, 0);
            a++;
            eqepREG1->QPOSCNT  =  0x00000000U;
        }
#endif
    }
//    gioSetBit(gioPORTA, 3, 1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

void wait(uint32 val)
{
    int i;
    for(i=0; i<val; i++)
        ;
}

// BREAK는 Active HIGH 동작 => HIGH를 주면 모터 멈춤
void check_break(void)
{
        gioSetBit(gioPORTB, 6, 1);
        gioSetBit(gioPORTA, 3, 0);
        wait(50000000);
        gioSetBit(gioPORTB, 6, 0);
        gioSetBit(gioPORTA, 3, 1);
}

void chg_pwm(etpwmBASE_t *etpwm, uint16 period, int duty)
{
    int cnt = 0;

    etpwmStopTBCLK();
    if(duty > 100)
    {
        duty = 100;
    }
    else if(duty < 0)
    {
        duty = 0;
    }
    etpwm->TBPRD = period - 1;
    cnt = (duty / 100) * period;
    etpwm->CMPA += cnt;

    etpwmStartTBCLK();
}

void sci_display_data(sciBASE_t *sci, uint8 *txt, uint32 len)
{
            while(len--)
            {
                uint8 txt1 = *txt++;
                    while((sciREG1->FLR & 0x4)== 4)
                        ;
                    sciSendByte(sciREG1, txt1);
            }
}

void sci_display_txt(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((sci->FLR & 0x4) == 0x4)
            ;
        sciSendByte(sci, *text++);
    }
}
/* USER CODE END */
