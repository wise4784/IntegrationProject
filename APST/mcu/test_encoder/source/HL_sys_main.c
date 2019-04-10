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

void wait(uint32); // 딜레이 함수

char buf[128] = {0};
unsigned int buflen = 0;

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    sciInit();
    sprintf(buf, "sci_Init\n\r\0");
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);
//    etpwmInit();
    QEPInit();

    sprintf(buf, "QEP_Init\t\n\r\0");
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);

    eqepEnableCounter(eqepREG1);
    eqepEnableGateIndex(eqepREG1);

    int cnt = 0;
    for(;;)
    {
        if((eqepREG1->QFLG & 0x400) == 0x400)
        {
            cnt = eqepREG1->QPOSILAT + 1;

            sprintf(buf, "CNT_Init : %d\t\n\r\0",cnt);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);
            eqepClearInterruptFlag(eqepREG1, QEINT_Iel);
        }
    }
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
