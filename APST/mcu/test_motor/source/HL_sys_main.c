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

/* USER CODE BEGIN (2) */
// UART 통신 함수
void sci_display_data(sciBASE_t *sci, uint8 *data, uint32 len);
void sci_display_txt(sciBASE_t *sci, uint8 *txt, uint32 len);

void wait(uint32); // 딜레이 함수

void check_break(void); // 브레이크 동작확인 함수 (Active High)

void chg_pwm(etpwmBASE_t *etpwm, uint32 pwm_freq, uint8 *duty); // pwm 클럭은 1Mhz
                                                                         // 모터 pwm 변경 함수
                                                                         // 주파수 = period * 0.000001
void const_velocity(int preCNT, int setCNT, int *error, float c_time); // 모터를 일정한 속도로 회전

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
float velocity; // 엔코더가 측정한 속도값
float ppr = 3000; // 엔코더 ppr

    // index(z상) 이 있으니까 1바퀴당 pcnt값 구할 수 있음.
    // ppr = 3000, 감속비 24, Quadrature 모드
    // 1 res의 pcnt = 3000 * 24 * 4
    // 1 res의 pcnt = 288000

#define Kp  2.63;
#define Ki  8.4;
#define Kd  0.00015;

// cmpa += kp*error;

float ierr;
float derr;

int pwm_CNT; // CMPA로 들어가는 누적값.
float P_term;
float I_term;
float D_term;
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    sciInit();
    sprintf(buf, "sci_Init\n\r\0");
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);

    gioInit();
    sprintf(buf, "gio_Init\n\r\0");
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);
    // VCLK3 - 37.5Mhz
    uint32 VCLK3_freq = 150000000 / ((systemREG2->CLK2CNTRL & 0x0000000FU) + 1);
//********************** EPWM 관련 *********************************//
    // PWM 주기 1ms 주파수 1khz
    etpwmInit();
    // PWM period : 1ms
    // etpwmREG4->TBPRD = (VCLK3_freq / 1000) - 1;
    float PWM_freq = etpwmREG4->TBPRD + 1;
    float duty = 0;
    sprintf(buf, "pwm_Init : %.2f Khz\t set Duty : %.1f%%\n\r\0", PWM_freq / 1000, duty);
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);
//*********************** EQEP 관련 *********************************//
    QEPInit();
    // Unit - 375khz
    float Unit_freq = VCLK3_freq / 100;
    // Count 초기화 주기 10ms
    eqepSetUnitPeriod(eqepREG1, Unit_freq);

    float ppd = 360 / (ppr * 24 * 4); // 엔코더 1ch 펄스당 degree

    float c_time = Unit_freq / VCLK3_freq;
    sprintf(buf, "QEP_Init\t set UNIT_TIME : %.2f msec\n\r\0", 1000 * c_time);
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);

    eqepEnableCounter(eqepREG1);
    eqepEnableUnitTimer(eqepREG1);

    // PPR이 맞는 지 확인하기위해 MAX 값 설정
    // eqepSetMaxPosnCount(eqepREG1, 3000*24*4);

    // 유저 버튼 누를때까지 대기
    while(gioGetBit(gioPORTB,4) == 1)
        ;
    wait(50000000);

    //****************** PID 관련 *************************************//
    int setCNT = 7200; // 20rpm으로 동작시키기 위한 10ms당 cnt값
    int error[2] = {0,0};



    for(;;)
    {
 //       cnt = eqepReadPosnCount(eqepREG1);
        // 제어주기(10ms) 마다 CNT 값 확인 코드
#if 1
        if((eqepREG1->QFLG & 0x800) == 0x800)
        {
//            gioSetBit(gioPORTA,5,1);
//            cnt = eqepReadPosnCount(eqepREG1);
//            eqepREG1->QPOSCNT  =  0x00000000U;
            pcnt = eqepReadPosnLatch(eqepREG1); // 정해놓은 시간동안 들어온 CNT 갯수
            velocity = ((float)pcnt * ppd / c_time) / 6; // rpm
            const_velocity(pcnt, setCNT, error, c_time);
/*
            cnt = eqepReadPosnCount(eqepREG1); Time Out 발생 시 확실하게 초기화됌.
 *
 *          sprintf(buf, "PCNT = %d\n\r\0", cnt);
 *          buflen = strlen(buf);
 *          sci_display_txt(sciREG1, (uint8 *)buf, buflen);
*/
            duty = pwm_CNT * 100 / PWM_freq;
            sprintf(buf, "CMPA = %d\t Duty = %.1f%%\n\r\0", pwm_CNT, duty);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

            sprintf(buf, "POSCNT = %d\n\r\0", pcnt);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

            sprintf(buf, "setCNT = %d,\t duty = %d\n\r\0", setCNT, (pcnt * 100) / setCNT);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

            sprintf(buf, "Velocity = %f\n\r\0", velocity);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

//          gioToggleBit(gioPORTB, 6);
//          gioToggleBit(gioPORTA, 5);
            // Flag가 자동 초기화가 안됌.
            eqepClearInterruptFlag(eqepREG1, QEINT_Uto);
        }
#endif
        //PWM 조절 확인 코드
#if 0
        if(gioGetBit(gioPORTB,4) == 0)
        {
            chg_pwm(etpwmREG4, PWM_freq, &duty);

            sprintf(buf, "Duty = %d\n\r\0", duty);
            buflen = strlen(buf);
            sci_display_txt(sciREG1, (uint8 *)buf, buflen);

            duty += 5;
            wait(50000000);
        }
#endif
#if 0
        // 전 엔코더 기준
        // 한 바퀴 펄스 갯수 테스트
       // POSCNT 1당 0.75도
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

void chg_pwm(etpwmBASE_t *etpwm, uint32 pwm_freq, uint8 *duty)
{
    if(*duty > 100)
    {
        *duty = 100;
    }

    etpwm->CMPA = *duty * pwm_freq / 100;

    sprintf(buf, "CNT = %d\n\r\0", etpwm->CMPA);
    buflen = strlen(buf);
    sci_display_txt(sciREG1, (uint8 *)buf, buflen);
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
