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
#include "FreeRTOS_UDP_IP.h"
#include "os_task.h"
#include "FreeRTOS_Sockets.h"

#include "HL_emac.h"
#include "HL_hw_reg_access.h"

/* Include gio header file - types, definitions and function declarations for system driver */
#include "HL_gio.h"
#include "HL_sys_pmu.h"

/* Console IO Support */
#include "HL_sci.h"
#include <string.h>
#include <stdio.h>

#define sciREGx  sciREG1

#define clearScreen() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (x), (y))

uint8_t    txtCRLF[]      = {'\r', '\n'};
char printBuffer [512];


void sciDisplayText(sciBASE_t *sci, uint8_t *text)
{

	size_t length = strlen ((const char *)text);

	while(length--)
    {
        while ((sci->FLR & 0x4) == 4);  //wait until busy
        sciSendByte(sci,*text++);      // send out text
    };
}

void sciDisplayTextLen(sciBASE_t *sci, uint8_t *text,uint32_t length)
{
	while(length--)
    {
        while ((sci->FLR & 0x4) == 4);  //wait until busy
        sciSendByte(sci,*text++);      // send out text
    };
}

#define DEBUG_STRING_BUFFER_SIZE 1024
static char cBuffer[DEBUG_STRING_BUFFER_SIZE];


/* Define Task Handles */
xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;
xTaskHandle xDebugTaskHandle;

/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        vTaskDelay(100);
    }
}
/* Task2 */
void vTask2(void *pvParameters)
{
    for(;;)
    {
        gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
        vTaskDelay(200);
    }
}

void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
	static BaseType_t xTasksAlreadyCreated = pdFALSE;

	char printBuffer[256];
	uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
	int8_t cBuffer[16];

    /* Check this was a network up event, as opposed to a network down event. */
    if( eNetworkEvent == eNetworkUp )
    {
        /* The network is up and configured.  Print out the configuration
        obtained from the DHCP server. */
        FreeRTOS_GetAddressConfiguration( &ulIPAddress,
                                          &ulNetMask,
                                          &ulGatewayAddress,
                                          &ulDNSServerAddress );

        /* Convert the IP address to a string then print it out. */
        FreeRTOS_inet_ntoa( ulIPAddress, cBuffer );
        sprintf(printBuffer,"IP Address: %s\r\n", cBuffer );
        sciDisplayText(sciREGx,(uint8_t *)printBuffer);

        /* Convert the net mask to a string then print it out. */
        FreeRTOS_inet_ntoa( ulNetMask, cBuffer );
        sprintf(printBuffer,"Subnet Mask: %s\r\n", cBuffer );
        sciDisplayText(sciREGx,(uint8_t *)printBuffer);

        /* Convert the IP address of the gateway to a string then print it out. */
        FreeRTOS_inet_ntoa( ulGatewayAddress, cBuffer );
        sprintf(printBuffer,"Gateway IP Address: %s\r\n", cBuffer );
        sciDisplayText(sciREGx,(uint8_t *)printBuffer);

        /* Convert the IP address of the DNS server to a string then print it out. */
        FreeRTOS_inet_ntoa( ulDNSServerAddress, cBuffer );
        sprintf(printBuffer,"DNS server IP Address: %s\r\n", cBuffer );
        sciDisplayText(sciREGx,(uint8_t *)printBuffer);

        /* Create the tasks that use the IP stack if they have not already been
        created. */
        if( xTasksAlreadyCreated == pdFALSE )
        {
            /*
             * For convenience, tasks that use FreeRTOS+UDP can be created here
             * to ensure they are not created before the network is usable.
             */

            xTasksAlreadyCreated = pdTRUE;
        }
    }
}

void vDebugTask(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(3000);

        vTaskGetRunTimeStats(cBuffer);

        /*
         * Clear the screen and move cursor back to top left and display the banner
         */

        sciDisplayText(sciREGx,"\033[H\033[JTask Execution Statistics\r\n-------------------------\r\n");

        /*
         * Display the stats
         */

        sciDisplayTextLen(sciREGx, (uint8_t *)cBuffer, DEBUG_STRING_BUFFER_SIZE);

    }
}


/*
 *  PMU Support
 */
void initialisePMU()
{
    /* PMU calibration */
	_pmuInit_();
    _pmuEnableCountersGlobal_();
    _pmuResetCounters_();
    _pmuStartCounters_(pmuCYCLE_COUNTER);
}

uint32 getPMUCycleCounter()
{
	return _pmuGetCycleCount_();
}

static uint32 totalCount;
static uint32 lastCycleCount;

void initExecutionTimer()
{
	initialisePMU();
	totalCount = 0;
	lastCycleCount = 0;
}

uint32 getExecutionTimer()
{
	uint32 cycleCount = getPMUCycleCounter();
	if (cycleCount < lastCycleCount)
	{
		/*
		 *  We have an overflow...
		 */

		totalCount += cycleCount + 	(0xFFFFFFFF - lastCycleCount);

	}
	else
	{
	   totalCount += (cycleCount-lastCycleCount)/1000;
	}

	lastCycleCount = cycleCount;

	return totalCount;
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

/* Define the network addressing.  These parameters will be used if either
 * ipconfigUDE_DHCP is 0 or if ipconfigUSE_DHCP is 1 but DHCP auto configuration
 * failed.
 * */
static const uint8_t ucIPAddress[ 4 ]		 = { 10, 42, 0, 30 };
static const uint8_t ucNetMask[ 4 ] 		 = { 255, 0, 0, 0 };
static const uint8_t ucGatewayAddress[ 4 ] 	 = { 10, 42, 0, 1 };

/*
 * The following is the address of an OpenDNS server.
 */
static const uint8_t ucDNSServerAddress[ 4 ] = { 208, 67, 222, 222 };

/* USER CODE END */

uint8	emacAddress[6U] = 	{0x0U, 0x08U, 0xEEU, 0x03U, 0xA6U, 0x6CU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */

    BaseType_t returnVal = pdFALSE;

	sciInit();
    sciDisplayText(sciREGx, txtCRLF);
    sciDisplayText(sciREGx, "Initialising GIO...\r\n");

	gioInit();


    sciDisplayText(sciREGx, "Initialising IP Layer...\r\n");

/*
 * Initialise the embedded Ethernet interface.  The tasks that use the
 * network are created in the vApplicationIPNetworkEventHook() hook function
 * below.  The hook function is called when the network connects.
 */

    returnVal = FreeRTOS_IPInit( ucIPAddress,
                     	 	 	 ucNetMask,
								 ucGatewayAddress,
								 ucDNSServerAddress,
								 emacAddress);

    if (pdFAIL == returnVal)
    {
        sciDisplayText(sciREGx, "...FAILED\r\n");
    }
    else
    {
        sciDisplayText(sciREGx, "...SUCCESS\r\n");
    }

    sciDisplayText(sciREGx, "Creating Tasks...\r\n");

    /* Create Task 1 */
    if (xTaskCreate(vTask1,"LowPri-LED2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 2 */
    if (xTaskCreate(vTask2,"HighPri-LED3", configMINIMAL_STACK_SIZE, NULL, 3, &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    if (xTaskCreate(vDebugTask,"DebugTask", configMINIMAL_STACK_SIZE*2, NULL,  1 | portPRIVILEGE_BIT  , &xDebugTaskHandle) != pdTRUE)
    {
        /* Task could not be created */
        sciDisplayText(sciREGx, "Error Creating Debug Task...\r\n");
    }

    sciDisplayText(sciREGx, "Starting Scheduler...\r\n");

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
