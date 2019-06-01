/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 02-Mar-2016
*   @version 04.05.02
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
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "FreeRTOS_UDP_IP.h"
#include "os_task.h"
#include "FreeRTOS_Sockets.h"
#include "os_semphr.h"

#include "HL_emac.h"
#include "HL_hw_reg_access.h"

/* Include eqep header file */
#include "HL_eqep.h"

/* Include eqep header file */
#include "HL_etpwm.h"

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

/********************************** EQEP를 이용한 PID코드 Initialize **********************************/

/*  Pin 설정
 *  GPIOA_3 -> BREAK
 *  GPIOA_5 -> Direction
 *  GPIOB_4 -> 유저 버튼
 *  GPIOB_6 -> 유저 LED
 *  etPWM4_A -> PWM (J10_26)
 *  eQEP1_A -> Hall_A
 *  eQEP1_B -> Hall_B
 */
char buf[128] = {0};
unsigned int buflen = 0;

uint32 pcnt;    // 엔코더 cnt값
float velocity; // 원판이 돌아가는 속도
float motor_vel; // 모터 엔코더 속도
float ppr = 3000.0; // 엔코더 ppr
float cpd; // EQEP pcnt 1당 엔코더 degree = 0.03
int setCNT = 7714; // 20rpm으로 동작시키기 위한 10ms당 cnt값
float c_time; // 제어 주기
float duty;
float PWM_freq;

#define Kp  2.63;
#define Ki  8.4;
#define Kd  0.00015;

int error[2];
float ierr;
float derr;

int pwm_CNT; // CMPA로 들어가는 누적값.
float P_term;
float I_term;
float D_term;

void const_velocity(int preCNT, int setCNT, int *error, float c_time) // 모터를 일정한 속도로 회전
{
    error[0] = setCNT - preCNT;
    ierr += (float)error[0] * c_time;
    derr = (float)(error[0] - error[1]) / c_time;
    P_term = (float)error[0] * Kp;
    I_term = ierr * Ki;
    D_term = derr * Kd;

    error[1] = error[0];

    pwm_CNT = (int)(P_term + I_term + D_term);

    if(pwm_CNT < 0)
    {
        pwm_CNT = -pwm_CNT;
    }
    if(pwm_CNT > 37500)
    {
        pwm_CNT = 37500;
    }

    etpwmREG4->CMPA = pwm_CNT;
}
/*************************************                 ****************************************/

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
xTaskHandle xTask3Handle;
xTaskHandle xDebugTaskHandle;

QueueHandle_t mutex = NULL;

/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(mutex, ( TickType_t ) 0x01) == pdTRUE)
        {
            xSocket_t socket = FreeRTOS_socket(FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP);
            if(socket == FREERTOS_INVALID_SOCKET)
            {
                sprintf(buf,"SOCKET is INVALID\r\n");
                sciDisplayText(sciREG1, (uint8_t *)buf);
            }
            gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
            gioToggleBit(gioPORTA, 5);

            xSemaphoreGive(mutex);
            // Delay는 ms 단위.
            vTaskDelay(100);
        }
    }
}

/* Task2 */
void vTask2(void *pvParameters)
{
    for(;;)
    {
        if(xSemaphoreTake(mutex, (TickType_t ) 0x01) == pdTRUE)
        {
            if((eqepREG1->QFLG & 0x800) == 0x800)
            {
                pcnt = eqepReadPosnLatch(eqepREG1); // 정해놓은 시간동안 들어온 CNT 갯수
                motor_vel = ((((float)pcnt * cpd / c_time) / 360.0) * 7.0 / 5.0) * 60.0; // 모터 엔코더 속도
                velocity = motor_vel / 24.0 / 7.5; // 원판 돌아가는 속도
                const_velocity(pcnt, setCNT, error, c_time);

                duty = pwm_CNT * 100 / PWM_freq;

                // Flag가 자동 초기화가 안됌.
                eqepClearInterruptFlag(eqepREG1, QEINT_Uto);
                vTaskDelay(10);
            }
        }
    }
}

/* Task3 */
void vTask3(void *pvParameters)
{
    for(;;)
    {
        sprintf(buf, "CMPA = %d\t Duty = %.1f%%\n\r\0", pwm_CNT, duty);
        sciDisplayText(sciREG1, (uint8 *)buf);

        sprintf(buf, "POSCNT = %d\n\r\0", pcnt);
        sciDisplayText(sciREG1, (uint8 *)buf);

        sprintf(buf, "setCNT = %d,\t duty = %d\n\r\0", setCNT, (pcnt * 100) / setCNT);
        sciDisplayText(sciREG1, (uint8 *)buf);

        sprintf(buf, "Motor_Velocity = %f\n\r\0", motor_vel);
        sciDisplayText(sciREG1, (uint8 *)buf);

        sprintf(buf, "Velocity = %f\n\r\0", velocity);
        sciDisplayText(sciREG1, (uint8 *)buf);

        vTaskDelay(300);
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

/* VCLK3 Frequency = 37.5Mhz */
    uint32 VCLK3_freq = 150000000 / ((systemREG2->CLK2CNTRL & 0x0000000FU) + 1);

//********************** EPWM 관련 *********************************//
    // PWM 주기 1ms 주파수 1khz
    etpwmInit();
    // PWM period : 1ms
    // etpwmREG4->TBPRD = (VCLK3_freq / 1000) - 1;
    PWM_freq = etpwmREG4->TBPRD + 1;
//    sprintf(buf, "pwm_Init : %.2f Khz\t set Duty : %.1f%%\n\r\0", PWM_freq / 1000, duty);
//    sciDisplayText(sciREG1, (uint8 *)buf);
    //*********************** EQEP 관련 *********************************//
    /* QUADRATURE mode
     * UnitTime initialize
     * Initialize period = 10ms
     * Unit time out Interrupt ON
     */
    QEPInit();
    // Unit - 375khz
    float Unit_freq = VCLK3_freq / 100;
    eqepSetUnitPeriod(eqepREG1, Unit_freq);
    c_time = Unit_freq / VCLK3_freq; // 10ms

    cpd = 360 / (ppr * 4); // degree per CNT

 //   sprintf(buf, "QEP_Init\t set UNIT_TIME : %.2f msec\n\r\0", 1000 * c_time);
 //   sciDisplayText(sciREG1, (uint8 *)buf);

    eqepEnableCounter(eqepREG1);
    eqepEnableUnitTimer(eqepREG1);

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
    vSemaphoreCreateBinary(mutex);

    /* Create Task 1 */
    if (xTaskCreate(vTask1,"LowPri-LED2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 2*/
    if (xTaskCreate(vTask2,"PID-Control", configMINIMAL_STACK_SIZE, NULL, 2, &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 3 */
    if (xTaskCreate(vTask3,"SCI-init", configMINIMAL_STACK_SIZE, NULL, 3, &xTask3Handle) != pdTRUE)
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
    while(1)
        ;

    return 0;
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
