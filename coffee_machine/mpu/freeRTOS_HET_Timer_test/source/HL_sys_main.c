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
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "os_queue.h"
#include "os_semphr.h"
#include "os_task.h"
#include "os_timer.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
/* Define Task Handles */
xTaskHandle xTask1Handle;

void wait_ms(uint32 delay){
    delay*=5000;//50us
    hetRAM1->Instruction[0].Data=0; // hetResetTimestamp()와 상동
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}


/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        /* Taggle HET[1] with timer tick */
        gioSetBit(hetPORT1, 17, gioGetBit(hetPORT1, 17) ^ 1);
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        wait_ms(5000);  // 6710*5000=33,550,000으로 2^25(33554432)와 매우가깝다.cnt_max뭔가 25비트와관련→ datasheet cnt부분에 max count 25bit라고 써 있다.
        wait_ms(5000);  // 50us * 5000 = 250.000 ms
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);

        wait_ms(5000);  // 6710*5000=33,550,000으로 2^25(33554432)와 매우가깝다.cnt_max뭔가 25비트와관련→ datasheet cnt부분에 max count 25bit라고 써 있다.
        wait_ms(5000);  //
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);

        wait_ms(5000);  // 6710*5000=33,550,000으로 2^25(33554432)와 매우가깝다.cnt_max뭔가 25비트와관련→ datasheet cnt부분에 max count 25bit라고 써 있다.
        wait_ms(5000);  //
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);

        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);

        wait_ms(5000);  // 6710*5000=33,550,000으로 2^25(33554432)와 매우가깝다.cnt_max뭔가 25비트와관련→ datasheet cnt부분에 max count 25bit라고 써 있다.
        wait_ms(5000);  //
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);

        wait_ms(5000);  // 6710*5000=33,550,000으로 2^25(33554432)와 매우가깝다.cnt_max뭔가 25비트와관련→ datasheet cnt부분에 max count 25bit라고 써 있다.
        wait_ms(5000);  //
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
        wait_ms(5000);
    }
}
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    hetInit();
    //hetInit();
    /* Set high end timer GIO port hetPort pin direction to all output */
    gioSetDirection(gioPORTB, 0xffffffff);
    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    /* Start Scheduler */
    vTaskStartScheduler();
    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
