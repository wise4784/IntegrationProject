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
#include "HL_system.h"
#include "HL_etpwm.h"
#include "HL_sci.h"
#include "HL_het.h"
#include "HL_ecap.h"

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
void init_all(void);
void sci_display(char* buf, int len);
void motor_bldc(void);
void wait_ms(int delay);

int throttle;
int init_back_flag=1;
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    char buf[128];
    int buf_len;

    init_all();

    sprintf(buf,"init Complete:\n\r\0");
    buf_len=strlen(buf);
    sci_display(buf,buf_len);

    for(;;){
        //motor_bldc();
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void wait_ms(int delay){
    delay*=5000;
    hetRAM1->Instruction[0].Data=0;
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}

void motor_bldc(void)
{
    char buf[64];
    int buf_len;

    sprintf(buf,"bldc_task start\n\r\0");
    buf_len=strlen(buf);
    sci_display(buf,buf_len);

    ecapDisableInterrupt(ecapREG1,ecapInt_CEVT3);
    ecapDisableInterrupt(ecapREG2,ecapInt_CEVT3);

    if(throttle>=950&&throttle<1450){
        if(init_back_flag){
            etpwmREG1->CMPA=1500;
            wait_ms(60);
            etpwmREG1->CMPA=1000;
            wait_ms(60);
            etpwmREG1->CMPA=1500;
            wait_ms(60);
            init_back_flag=0;

            sprintf(buf,"init back\n\r\0");
            buf_len=strlen(buf);
            sci_display(buf,buf_len);
        }
        throttle=1450;
    }else if(throttle>=1550&&throttle<=2000){
        throttle=1550;
        init_back_flag=1;
    }else{
        throttle=1500;
    }
    etpwmREG1->CMPA=throttle;
    wait_ms(60);

    sprintf(buf,"bldc_task_end CMPA:%d\n\r\0",throttle);
    buf_len=strlen(buf);
    sci_display(buf,buf_len);

    ecapEnableInterrupt(ecapREG1,ecapInt_CEVT3);
    //ecapEnableInterrupt(ecapREG2,ecapInt_CEVT3);
}

void sci_display(char* buf, int len){
    for(;len>0;len--){
        sciSendByte(sciREG1,*buf++);
    }
}

void init_all(void){
    etpwmInit();
    ecapInit();
    hetInit();
    sciInit();
    _enable_interrupt_();
    etpwmStartTBCLK();
    ecapStartCounter(ecapREG1);
   // ecapStartCounter(ecapREG2);
    ecapEnableCapture(ecapREG1);
    //ecapEnableCapture(ecapREG2);
    init_back_flag=1;
}

void ecapNotification(ecapBASE_t *ecap, uint16 flag)
{
    int cap[2];
    char buf[128];
    int buf_len;

    sprintf(buf,"cap Start\n\r\0");
    buf_len=strlen(buf);
    sci_display(buf,buf_len);

    cap[0]=ecapREG1->CAP1;
    cap[1]=ecapREG1->CAP2;
    throttle=(cap[1]-cap[0])/VCLK3_FREQ;

    if(throttle<=950||(throttle>1450&&throttle<1550)){
        throttle=1500;
    }

    sprintf(buf,"cap input CAP1(throttle):%d\n\r\0",throttle);
    buf_len=strlen(buf);
    sci_display(buf,buf_len);

    if(throttle>=950&&throttle<1450){
            if(init_back_flag){
                etpwmREG1->CMPA=1500;
                wait_ms(100);
                etpwmREG1->CMPA=1000;
                wait_ms(100);
                etpwmREG1->CMPA=1500;
                wait_ms(100);
                init_back_flag=0;
                sprintf(buf,"---------init back-----------\n\r\0");
                buf_len=strlen(buf);
                sci_display(buf,buf_len);
            }
            throttle=1450;
        }else if(throttle>=1550&&throttle<=2000){
            throttle=1550;
            init_back_flag=1;
        }else{
            throttle=1500;
        }
        etpwmREG1->CMPA=throttle;
        wait_ms(100);

    sprintf(buf,"cap output throttle:%d\n\r\0",throttle);
    buf_len=strlen(buf);
    sci_display(buf,buf_len);
}
/* USER CODE END */
