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
#include "os_task.h"
#include "os_semphr.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_system.h"
#include "HL_het.h"
#include "HL_esm.h"

#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_eqep.h"
#include "HL_etpwm.h"
#include "HL_adc.h"
#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "ipv4/lwip/ip_addr.h"
#include "lwip/udp.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void ctrlTask(void *pvParameters);
void pidTask(void *pvParameters);
void stepTask(void *pvParameters);
void udpTask(void *pvParameters);
void sciTask(void *pvParameters);
void imuTask(void *pvParameters);
void adcTask(void *pvParameters);
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
extern void EMAC_LwIP_Main (uint8_t * emacAddress);

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port);
void const_velocity(int preCNT, int setCNT, int *error, float c_time);
void fl2in(float a, int *b);
void wait(uint32 delay);
void reload(uint8 cnt);

void set_Status(uint8 bit); /* bit0 : Charge / bit1 : 각도 */
void reset_Status(uint8 bit);

void convert(unsigned char *ch, unsigned char *fl); /* ASCII to float */
void imu_check(void);

float getAvg(int *buf, int value);  // MoveAvgFilter

#define VOLTAGE     0
#define DEGREE      1
#define MCU_READY   2
#define MCU_GO      3

/* Define Task Handles */
xTaskHandle CtrlTaskHandle;
xTaskHandle PidTaskHandle;
xTaskHandle StepTaskHandle;
xTaskHandle SciTaskHandle;
xTaskHandle UdpTaskHandle;
xTaskHandle ImuTaskHandle;
xTaskHandle AdcTaskHandle;

/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#define SCI_DEBUG   0 // If this value set 1, sciREG1 prints Debug messages.
#define I2C_DEBUG   0 // If this value set 1, I2C_2 & MPU will work
#define IMU_DEBUG   1 // If this value set 1, AHRSv1 will work
#define AHRS_SET    0 // If this value set 1, IMU setting : No period Data, flash save.
                         // 센서를 처음 실행시킬 때 한번만 하면 됩니다. 다음부턴 안해도 센서 메모리에 저장되서 동작합니다.
#define DSP_DEBUG   0 // DSP 통신 테스트 If this value set 1, it works for TEST MODE
#define ADC_DEBUG   1 // For ADC checking. If this value set 1, it works.

#if SCI_DEBUG
uint8_t sciTest[] = {"SCI very well\r\n"};
uint8 length = sizeof(sciTest);
uint8_t txtCRLF[] = {'\r', '\n'};
uint8_t txtInit[] = {"All peripheral Initialize\n\r"};
uint8_t txtlwIP[] = {"lwIP Initializing......\n\r"};
uint8_t txtSys[] = {"ALL SYSTEM START !!!!!!!! \n\r"};
uint8_t txtTask[] = {"Task START !!!!!\n\r"};
uint8_t txtOK[] = {"Success!!!\n\r"};
#endif

/* for Velocity */
char vbuf[32] = {0};
unsigned int vbuflen = 0;
/* for CMPA */
char cbuf[32] = {0};
unsigned int cbuflen = 0;

/*********************************************************** For PID ***********************************************************************/
int pwm_CMPA;
char buf[32] = {0};
unsigned int buflen = 0;

/* eQEP, PID 변수 */
uint32 pcnt;    // 엔코더 cnt값
float velocity; // 엔코더가 측정한 속도값
float ppr = 3000; // 엔코더 ppr
// ppr = 3000, 감속비 24, Quadrature 모드
// 1 res의 pcnt = 3000 * 24 * 4
// 1 res의 pcnt = 288000
int setCNT; // 20rpm으로 동작시키기 위한 10ms당 cnt값
int error[2] = {0,0};

// Count 초기화 주기 10ms
float Unit_freq = VCLK3_FREQ * 10000.0;
// eqepSetUnitPeriod(eqepREG1, Unit_freq);

// 엔코더 1ch 펄스당 degree
float ppd = 0.00125; //   360 / (ppr * 24 * 4)
// 제어주기
float c_time = 0.01; //   Unit_freq / (VCLK3_FREQ * 1000000.0)

#define Kp  2.63;
#define Ki  8.4;
#define Kd  0.00015;

float ierr;
float derr;

float P_term;
float I_term;
float D_term;
/*******************************************************************************************************************************************/

/*********************************************************** For UDP ***********************************************************************/
struct udp_pcb *pcb;

char udp_msg[3] = {0};
uint8_t err_bind[] = {"bind error!!!\r\n"};
struct pbuf *p;

/* 표시할 항목
     * 충전상태 ( BIT 0 )
     *          0: 충전중
     *          1: 완충(700V)
     * 각도 ( BIT 1 )
     *          0: 조준 중
     *          1: 조준 완료
     * READY( BIT 2 )
     *          0: Task don't start
     *          1: Task Start!
     *
 */
uint8 status_flag; /* 1 : Charge / 2 : 각도  / 3 : All Ready / 4 : Go */

/*******************************************************************************************************************************************/

/*********************************************************** For AHRS **********************************************************************/
// 플래쉬 저장
uint8_t fw_save[] = {0x02,0x09,0x02,0x18,0x07,0x00,0x00,0x01,0x00,0x00,0x00,0x22,0x03};
// 오일러 각 중 roll 데이터 주셈
uint8_t send_roll[] = {0x02,0x09,0x02,0x3c,0x35,0x00,0x01,0x00,0x00,0x00,0x00,0x74,0x03};
// rs232 동기화 데이터 보내지 마라
uint8_t period_stop[] = {0x02,0x09,0x02,0x18,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x2f,0x03};
uint8_t rx[13] = {0};

int setDGR; // DSP에서 준 각도 값 ( 100배 뻥튀기 해서 옴 )
float setRoll; // (float)setDGR / 100.0
float roll; // IMU 측정값
float angError;

/*******************************************************************************************************************************************/


/*********************************************************** For PWM ***********************************************************************/

/* SERVO */
uint16 servo_pwm = 420; // 420 = 0 degree,  1910 = 180 degree

/* STEP */
uint32 step_cnt;
#define STEP       1024.2 // 각도-cnt 계수 값.
/*******************************************************************************************************************************************/


/*********************************************************** FOR ADC ***********************************************************************/

/* ADC1_9 */
#define vCoeff  0.24176              //cap_bank voltage-coefficient
#define dSize   10                   // data 10

float cVolt;
int dBuf[dSize] = {0};

uint16 adc_val;

adcData_t adc_data[2];

/*******************************************************************************************************************************************/


/* USER CODE END */

uint8	emacAddress[6U] = 	{0x00U, 0x08U, 0xeeU, 0x03U, 0xa6U, 0x6cU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
    /*clear the ESM error manually*/
    esmREG->SR1[2] = 0xFFFFFFFFU;
    esmREG->SSR2   = 0xFFFFFFFF;
    esmREG->EKR = 0x0000000A;
    esmREG->EKR = 0x00000000;

    /* gio Pin initialize */
    gioInit();
    /* het Pin initialize */
    hetInit();

    /* clear MCU status */
    status_flag = 0x00;
    sciInit();

    //VCLK3_FREQ; // 37.500F
    etpwmInit();
    QEPInit();

    etpwmStartTBCLK();
    eqepEnableCounter(eqepREG1);
    eqepEnableUnitTimer(eqepREG1);

    /* adc Initialize */
#if ADC_DEBUG
    adcInit();
    adcStartConversion(adcREG1, adcGROUP1);
#endif
#if SCI_DEBUG
    sciSend(sciREG1, sizeof(txtInit), txtInit);
    sciSend(sciREG1, sizeof(txtOK), txtOK);
#endif

    /* 전원 (PW_sw)를 안누르면 동작 X.
     * 전원버튼 누르면 A0에 LOW가 들어오고 while문 벗어남.
   */
#if DSP_DEBUG // for DEBUG
    while(gioGetBit(gioPORTB, 4))
        ;
#else
    while(gioGetBit(gioPORTA, 0))
        ;
#endif
    // Almost  1 sec
    wait(5000000);

    /* ALL DEVICE, System ON! */
    gioSetPort(gioPORTA, 0xFE);

#if SCI_DEBUG
    sciSend(sciREG1,sizeof(txtSys), txtSys);
#endif

#if SCI_DEBUG
    sciSend(sciREG1, sizeof(txtlwIP), txtlwIP);
#endif
    EMAC_LwIP_Main(emacAddress);
#if SCI_DEBUG
    sciSend(sciREG1, sizeof(txtOK), txtOK);
#endif

    pcb = udp_new();
#if 1
    udp_bind(pcb, IP_ADDR_ANY, 7777);
#if SCI_DEBUG
        sciSend(sciREG1, sizeof(err_bind), err_bind);
#endif
    udp_recv(pcb, udp_echo_recv, NULL);
#endif
    wait(1000);
    set_Status(MCU_READY); // MCU Initialize finish.

    while(!(status_flag & 0x08))
    {
        p = pbuf_alloc(PBUF_TRANSPORT, 3, PBUF_RAM);
        udp_msg[0] ='s';
        udp_msg[1] = status_flag;
        udp_msg[2] = 0;
        memcpy(p->payload, udp_msg, sizeof(udp_msg));
        udp_sendto(pcb, p, IP_ADDR_BROADCAST, 7777);
        pbuf_free(p);

        wait(10000);
    }

#if SCI_DEBUG
    sciSend(sciREG1, sizeof(txtTask), txtTask);
#endif

    // ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT)
    /* Create Task 1 */
    //    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
#if 0
    if (xTaskCreate(ctrlTask,"Control", configMINIMAL_STACK_SIZE, NULL, 1, &CtrlTaskHandle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
#endif
    /* Create Task 2 */
#if 1
    if (xTaskCreate(pidTask,"PID", configMINIMAL_STACK_SIZE, NULL, 6, &PidTaskHandle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
#endif
    /* Create Task 3 */
#if 1
    if(xTaskCreate(stepTask, "STEP", configMINIMAL_STACK_SIZE, NULL, 2, &StepTaskHandle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 4 */
#if SCI_DEBUG
    if(xTaskCreate(sciTask, "SCI", 2 * configMINIMAL_STACK_SIZE, NULL, 7, &SciTaskHandle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 5 */
#if 1
    if(xTaskCreate(udpTask, "UDP", 4 * configMINIMAL_STACK_SIZE, NULL, 8, &UdpTaskHandle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Create Task 6 */
#if AHRS_SET
    if(xTaskCreate(imuTask, "IMU", configMINIMAL_STACK_SIZE, NULL, 5, &ImuTaskHandle) != pdTRUE)
    {
        while(1);
    }
#endif
#if ADC_DEBUG
    if(xTaskCreate(adcTask, "ADC", configMINIMAL_STACK_SIZE, NULL, 4, &AdcTaskHandle) != pdTRUE)
    {
        while(1);
    }
#endif
    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */

/* Task1 */
void ctrlTask(void *pvParameters)
{
    /*
     * All Task control.
     * Task들을 일시정지 시키거나 재시작 시키는 Task
     */
    //vTaskSuspend(StepTaskHandle); // Step 모터 테스크 일시정지.

#if 1
    for(;;)
    {
        taskENTER_CRITICAL();
            /* Taggle GIOB[6] with timer tick */
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
#if 0
        if(1)
            ;
        vTaskSuspend(SciTaskHandle);
#endif
        taskEXIT_CRITICAL();

        vTaskDelay(300);
    }
#endif
}

void pidTask(void *pvParameters)
{
    /*
     * If you want to know position CNT value,
     * Use this Function 'eqepReadPosnCount(eqepREG1)'
     * but value clear every 10ms later.
     */
    for(;;)
    {
        taskENTER_CRITICAL();
        // 10ms마다 플래그 set
        if((eqepREG1->QFLG & 0x800) == 0x800)
        {
            pcnt = eqepReadPosnLatch(eqepREG1); // 정해놓은 시간동안 들어온 CNT 갯수
            velocity = ((float)pcnt * ppd / c_time) / 6.0; // rpm
            const_velocity(pcnt, setCNT, error, c_time);
            // Flag가 자동 초기화가 안됌.
            eqepClearInterruptFlag(eqepREG1, QEINT_Uto);
        }
        taskEXIT_CRITICAL();
        vTaskDelay(10);
    }
}

#if 1
void stepTask(void *pvParameters)
{
    int i;
    for(;;)
    {
        vTaskSuspend(NULL);

        taskENTER_CRITICAL();
        while(!(status_flag & 0x02))
        {
            imu_check();
            angError = setRoll - roll;

            if(angError < 0)
            {
                // 포대 방향 아래로 -> step 모터는 위로
                gioSetBit(gioPORTB, 3, 1);  // 방향은 실험해봐야 함.
                if(angError > -0.1)
                {
                    // step모터 Stop
                      // 조준 완료
                    set_Status(DEGREE);
                }
                else
                {
                    step_cnt = -1 * angError * STEP;
                    for(i = 0; i < step_cnt * 2; i++)
                    {
                        gioToggleBit(gioPORTA, 2);
                        wait(500);
                    }
                }
            }
            else
            {
                // 포대 방향 위로 -> step 모터는 아래로
                gioSetBit(gioPORTB, 3, 0);

                if(angError < 0.1)
                {
                    // step모터 Stop
                      // 조준 완료
                    set_Status(DEGREE);
                }
                else
                {
                    step_cnt = angError * STEP;
                    for(i = 0; i < step_cnt * 2; i++)
                    {
                        gioToggleBit(gioPORTA, 2);
                        wait(500);
                    }
                }
            }
        }
        taskEXIT_CRITICAL();
    }
}
#endif

#if SCI_DEBUG
void sciTask(void *pvParameters)
{
    /* SCI Baud Rate : 230400
     * sciREG1
     */
#if 1
    sprintf(buf, "\033[H\033[JSCI_Baud_Rate = 230400\n\r\0");
    buflen = strlen(buf);
    sciSend(sciREG1, buflen, (uint8 *)buf);
    sciSend(sciREG1, sizeof(txtCRLF), txtCRLF);
    sciSend(sciREG1, length, sciTest);
#endif
    for(;;)
    {
        taskENTER_CRITICAL();
#if 0
        sprintf(buf, "CMPA = %d\t setCNT = %d\n\r\0", pwm_CMPA, setCNT);
        buflen = strlen(buf);
        sciSend(sciREG1, buflen, (uint8 *)buf);

        sprintf(buf, "Velocity = %d\n\r\0", (int)velocity);
        buflen = strlen(buf);
        sciSend(sciREG1, buflen, (uint8 *)buf);
#endif

#if 0
        printf("CMPA = %d\n", pwm_CMPA);
        printf("Velocity = %.2f\n", velocity);
#endif

#if SCI_DEBUG & I2C_DEBUG

        sprintf(buf, "roll = %d.%d \t pitch = %d.%d \t yaw = %d.%d \n\r\0",
                apa[0],apa[1], ara[0],ara[1], yd[0],yd[1]);
        buflen = strlen(buf);
        sciSend(sciREG1, buflen, (uint8 *) buf);
#endif

#if SCI_DEBUG
        vbuflen = strlen(vbuf);
        sciSend(sciREG1, vbuflen, (uint8 *) vbuf);
#endif
        taskEXIT_CRITICAL();
        vTaskDelay(500);
    }
}
#endif

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    if (p != NULL)
    {
        char *rx_pk = p->payload;
        switch(rx_pk[0])
        {
            case 's' :
                /* DSP에서 CNT (속도) 값만 보내줌 */
                setCNT = rx_pk[1] << 24U |
                         rx_pk[2] << 16U |
                         rx_pk[3] << 8U  |
                         rx_pk[4];
#if SCI_DEBUG
            sprintf(vbuf,"%d,%d\n\r",setCNT,setDGR);
#endif
                break;

                /* MCU가 준비되서 Ready signal을 전송하면 DSP에서 받고 준비되면 'g'를 보내서 MCU 전체 테스크 동작 시작. */
            case 'g' :
                set_Status(MCU_GO);
#if DSP_DEBUG
                set_Status(DEGREE);
                set_Status(VOLTAGE);
#endif
                break;

            case 't' :
                /* DSP의 Trigger Signal. 레일건 (gio A? B? n bit SET!) */
                gioSetBit(gioPORTB, 0, 0);
                wait(100);
                gioSetBit(gioPORTB, 0, 1);

                /* Volatge flag reset, 재장전 */
#if !DSP_DEBUG
                reset_Status(VOLTAGE);
                // reload(val) : 1 =< val =< 190
                reload(50);
#endif
                break;

            case 'd' :
                /* DSP에서 속도 + 각도 보내준 것. */
                setCNT = rx_pk[1] << 24U |
                         rx_pk[2] << 16U |
                         rx_pk[3] << 8U  |
                         rx_pk[4];
                setDGR = rx_pk[5] << 24U |
                         rx_pk[6] << 16U |
                         rx_pk[7] << 8U  |
                         rx_pk[8];

                setRoll = (float)setDGR / 100.0;
#if !DSP_DEBUG
                vTaskResume(StepTaskHandle);
#endif
#if SCI_DEBUG
            sprintf(vbuf,"%d,%d\n\r",setCNT,setDGR);
#endif
                break;

            case 'l' :
                // laser on 레이저 발사.
                gioSetBit(gioPORTB, 1, 1);
                break;

            case 'o' :
                // laser off
                gioSetBit(gioPORTB, 1, 0);
                break;

            default:
                /* 다른값이 날라오면 에러  or 재요청 (미구현) */
                break;
        }
        pbuf_free(p);
    }
}

void udpTask(void *pvParameters)
{
    for(;;)
    {
        taskENTER_CRITICAL();

        p = pbuf_alloc(PBUF_TRANSPORT, sizeof(udp_msg), PBUF_RAM);

        udp_msg[0] = 's';
        udp_msg[1] = status_flag;
        udp_msg[2] = 0;

        memcpy(p->payload, udp_msg, sizeof(udp_msg));
        udp_sendto(pcb, p, IP_ADDR_BROADCAST, 7777);
        pbuf_free(p);

        taskEXIT_CRITICAL();

        vTaskDelay(10);
    }
}

void adcTask(void *pvParameters)
{
    for(;;)
    {
        while((adcIsConversionComplete(adcREG1, adcGROUP1))==0)
            ;

        adcGetData(adcREG1, adcGROUP1, &adc_data[0]);

        adc_val = adc_data[0].value;
        cVolt = getAvg(dBuf, adc_val);

        vTaskDelay(10);
    }
}
void wait(uint32 delay)
{
    int i;

    for (i = 0; i < delay; i++)
        ;
}
void const_velocity(int preCNT, int setCNT, int *error, float c_time)
{
    error[0] = setCNT - preCNT;
    ierr += (float)error[0] * c_time;
    derr = (float)(error[0] - error[1]) / c_time;
    P_term = (float)error[0] * Kp;
    I_term = ierr * Ki;
    D_term = derr * Kd;

    error[1] = error[0];

    pwm_CMPA = (int)(P_term + I_term + D_term);

    if(pwm_CMPA < 0)
    {
        pwm_CMPA = -pwm_CMPA;
    }
    if(pwm_CMPA > 37500)
    {
        pwm_CMPA = 37500;
    }
    etpwmREG1->CMPA = pwm_CMPA;
}
#if AHRS_SET
void imuTask(void *pvParameters)
{
    taskENTER_CRITICAL();

    while(!sciIsTxReady(sciREG3))
        ;
    sciSend(sciREG3, sizeof(period_stop),period_stop);

    while(!sciIsRxReady(sciREG3))
        ;
    sciReceive(sciREG3, sizeof(rx), rx);
    printf("RS232 No data : ");
    for(i = 0; i < sizeof(rx); i++)
        {
            printf("%2x",rx[i]);
        }
    printf("\n");
        while(!sciIsTxReady(sciREG3))
            ;
        sciSend(sciREG3, sizeof(fw_save), fw_save);

        while(!sciIsRxReady(sciREG3))
            ;
        sciReceive(sciREG3, sizeof(rx), rx);

        printf("Flash save : ");
        for(i = 0; i < sizeof(rx); i++)
        {
            printf("%2x",rx[i]);
        }
        printf("\n");

        taskEXIT_CRITICAL();
    for (;;)
    {
        taskENTER_CRITICAL();

        while(!sciIsTxReady(sciREG3))
            ;
        sciSend(sciREG3, sizeof(send_roll), send_roll);

        while(!sciIsRxReady(sciREG3))
            ;
        sciReceive(sciREG3, sizeof(rx), rx);

        taskEXIT_CRITICAL();
        vTaskSuspend(NULL);
    }
}
#endif
void set_Status(uint8 bit)
{
    status_flag |= 0x1U << bit;
}

void reset_Status(uint8 bit)

{
    status_flag  = status_flag &~(0x1U << bit);
}

void convert(unsigned char *arr, unsigned char *num)
{
    int i;
    for(i = 0; i < 4; i++)
        num[i]  = arr[10 - i];
}

void imu_check(void)
{
#if SCI_DEBUG & 0
        while(!sciIsTxReady(sciREG1))
                    ;
        sciSend(sciREG1, sizeof(rx), rx);
        while(!sciIsTxReady(sciREG1))
                    ;
        sciSend(sciREG1, sizeof(txtCRLF),txtCRLF);
#endif
#if 0
        printf("roll data : ");
        for(i = 0; i < sizeof(rx); i++)
        {
            printf("%2x",rx[i]);
        }
        printf("\n");
#endif

#if 0
        printf("data : %f\n", roll);
        /*
        sprintf(buf,"roll = %f\r\n", roll);
        buflen = strlen(buf);

        while(!sciIsTxReady(sciREG1))
                ;
        sciSend(sciREG1, buflen, buf);
        */
#endif

    while(!sciIsTxReady(sciREG3))
        ;
    sciSend(sciREG3, sizeof(send_roll), send_roll);

    while(!sciIsRxReady(sciREG3))
        ;
    sciReceive(sciREG3, sizeof(rx), rx);
    /*
        roll = rx_data2[10] << 24
                | rx_data2[9] << 16
                | rx_data2[8] << 8
                | rx_data2[7];
    */
    convert(rx, &roll);
}

void reload(uint8 cnt)
{
    cnt = cnt * 10;
    servo_pwm += cnt;
    if(servo_pwm > 1910)
    {
        servo_pwm = 1910;
    }
    etpwmREG4->CMPA = servo_pwm;
    wait(300000 * (cnt + 1));
    servo_pwm = 420;
    etpwmREG4->CMPA = servo_pwm;
}

float getAvg(int *buf, int value)
{
    int i, total;

    for(i = 0; i < dSize-1; i++)
     {
            buf[i] = buf[i+1];
     }
     buf[dSize-1] = value;

     for(i = 0; i < dSize; i++)
     {
         total += buf[i];

     }
    return (((total/dSize) * vCoeff)+24.703);           //0.2298x+14.703
}
/* USER CODE END */
