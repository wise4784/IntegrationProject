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
#include "HL_sys_common.h"
#include "HL_sys_core.h"
#include "HL_sys_vim.h"
#include "HL_system.h"
#include "HL_esm.h"
#include "HL_rti.h"
#include "HL_gio.h"
#include "HL_sci.h"

#include <stdio.h>
#include <string.h>
/* USER CODE END */



/* Include Files */







/* USER CODE BEGIN (1) */





void _disable_all_(void);

void _gio_a5_on_(void);

int _print_cpsr_(void);
int _print_spsr_(void);

#define UART        sciREG1
//#define vimRAM_RTI ((vimRAM_t *)0xFFF8200CU)

uint32_t cnt;

/* USER CODE END */



/** @fn void main(void)

*   @brief Application main function

*   @note This function is empty by default.

*

*   This function is called after startup.

*   The user can use this function to implement the application.

*/



/* USER CODE BEGIN (2) */

void wait(uint32 delay)
{
    int i;
    for(i = 0; i < delay; i++)
        ;
}



void sci_display(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((UART->FLR & 0x4) == 4)
            ;
        sciSendByte(UART, *text++);
    }
}



void disp_set(char *str)
{

    char buf[128] = {0};
    unsigned int buf_len;

    sprintf(buf, str);
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
    wait(100000);
}
/* USER CODE END */



int main(void)
{
/* USER CODE BEGIN (3) */

    int cpsr_data;
    int spsr_data;
    char txt_buf[256] = {0};
    unsigned int buf_len;

#if 0

    gioInit();

    gioSetDirection(gioPORTA, 0x00000020);

    gioSetPort(gioPORTA, 0x00000020);

#endif

    _gio_a5_on_();

#if 1
    sciInit();
    disp_set("sci config success!!!\n\r\0");

    rtiInit();
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);

    _enable_interrupt_();
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);

    for(;;)
    {
        cpsr_data = _print_cpsr_();
        sprintf(txt_buf, "cpsr = 0x%x\n\r\0", cpsr_data);
        buf_len = strlen(txt_buf);
        sci_display(sciREG1, (uint8 *) txt_buf, buf_len);
        //wait(4000000);

        spsr_data = _print_spsr_();
        sprintf(txt_buf, "spsr = 0x%x\n\r\0", spsr_data);
        buf_len = strlen(txt_buf);
        sci_display(sciREG1, (uint8 *) txt_buf, buf_len);
        //wait(10000000);

        if(cnt == 11)
        {
            int *addr;
            _disable_all_();
            vimREG->REQMASKSET0 = (uint32)((uint32)1U << 0U)
                                    | (uint32)((uint32)1U << 1U)
                                    | (uint32)((uint32)0U << 2U);

            vimREG->FIRQPR0 = (uint32)((uint32)SYS_FIQ << 0U)
                                | (uint32)((uint32)SYS_FIQ << 1U)
                                | (uint32)((uint32)SYS_IRQ << 2U);

            addr = 0xFFF8200CU;
            *addr = NULL;
        }
    }
#endif
    return 0;
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
    cnt++;
    gioSetPort(gioPORTA, gioGetPort(gioPORTA) ^ 0x00000020);
        //_disable_IRQ_interrupt_();
}

/* USER CODE END */
