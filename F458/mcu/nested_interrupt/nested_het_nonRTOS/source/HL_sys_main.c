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
#include "HL_sci.h"
#include "HL_het.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void sci_display(sciBASE_t* sci, uint8* buf, int len);
void hetRAMinit(void);
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    hetInit();
    hetRAMinit();
    sciInit();

    _enable_IRQ();

    for(;;)
        ;
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

void het2LowLevelInterrupt(void){
    int len;
    char buf[64];
    int tmp;

    hetREG2->FLG=1;

    sprintf(buf,"*****************het2 start*****************\n\r\0");
    len=strlen(buf);
    sci_display(sciREG1,(uint8*)buf,len);


    tmp=5*1000*1000*2;


    while((hetRAM2->Instruction[0].Data>>7)<tmp)
        ;

    sprintf(buf,"*****************het2 end*****************\n\r\0");
    len=strlen(buf);
    sci_display(sciREG1,(uint8*)buf,len);

}

void het1LowLevelInterrupt(void)
{
    int len;
    char buf[64];
    int tmp;

    hetREG1->FLG=1;

    sprintf(buf,"----------------het1 start--------------\n\r\0");
    len=strlen(buf);
    sci_display(sciREG1,(uint8*)buf,len);

    tmp=5*1000*1000*0.5;

    while((hetRAM1->Instruction[0].Data>>7)<tmp)



    sprintf(buf,"----------------het1 end-------------\n\r\0");
    len=strlen(buf);
    sci_display(sciREG1,(uint8*)buf,len);
}

void hetRAMinit(){
    int sec=5*1000*1000;
    int cnt1=1, cnt2=4;

    hetREG2->FLG=1;

    cnt1=sec*cnt1;
    cnt2=sec*cnt2;

    hetRAM1->Instruction[0].Program=0x00000C81U;
    hetRAM1->Instruction[0].Control=cnt1;
    hetRAM1->Instruction[0].Data=0;

    hetRAM2->Instruction[0].Program=0x00000C81U;
    hetRAM2->Instruction[0].Control=cnt2;
    hetRAM2->Instruction[0].Data=0;

    hetREG1->INTENAS=1;
    hetREG2->INTENAS=1;

}


void sci_display(sciBASE_t* sci, uint8* buf, int len){
    while(len--){
        while(sci->FLR&0x4==4)
            ;
        sciSendByte(sci,*buf++);
    }
}

/* USER CODE END */
