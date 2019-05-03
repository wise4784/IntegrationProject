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
#include "HL_sys_core.h"
#include "HL_sys_vim.h"
#include "HL_system.h"
#include "HL_esm.h"
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include "HL_gio.h"
#include "HL_rti.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void _gio_init_(void);
void _gio_A5_on_(void);
void _gio_A5_off_(void);
void _disable_ALL_interrupt_(void);

int cnt =0;
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
#if 0
      gioInit();
      gioSetDirection(gioPORTA,0x00000020);
      gioSetBit(gioPORTA,5,0x01);
#endif
      _gio_init_();
      rtiInit();
      rtiEnableNotification(rtiREG1,rtiNOTIFICATION_COMPARE0);
      _enable_interrupt_();

      rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);
      for(;;)
      {
          if(cnt >=11)
          {
              int *addr;
              _disable_ALL_interrupt_();

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
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
    cnt++;

    if((gioGetPort(gioPORTB)^0x00000040) & 0x00000040)
        _gio_A5_on_();
    else
        _gio_A5_off_();
}
/* USER CODE END */
