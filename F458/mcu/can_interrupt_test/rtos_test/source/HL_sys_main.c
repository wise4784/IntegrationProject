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
#include "HL_system.h"
#include "HL_etpwm.h"
#include "HL_sci.h"
#include "HL_het.h"
#include "HL_ecap.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"
#include "os_queue.h"
#include "HL_can.h"

#include <stdio.h>
#include <string.h>

#define sciREGx sciREG1
#define DCNT 8
#define __TEST__//for test(rf control) or automatic driving(line trace)


/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

//functions
void init_all(void);
void sci_display(sciBASE_t *sci, char* buf, int len);
void sci_displayISR(sciBASE_t *sci, char* buf, int len);
void wait_ms(int num, int delay);
void hetRamInit(void);


void canIntDisable(canBASE_t *node);
void canIntEnable(canBASE_t *node);


//tasks
void init_task(void * pbParameters);//initalize peripheral (related interrupt)
void motor_bldc(void* pbParameters);
void motor_servo(void* pbParameters);
void pid(void* pbParameter);
void send_msg(void* pbParameter);


uint8_t can_recv[DCNT];
uint8_t can_tran[DCNT];

int throttle=1500;
int rudder=1250;
int init_back_flag=1;

TaskHandle_t bldc_Handle;
TaskHandle_t servo_Handle;
TaskHandle_t pid_Handle;
TaskHandle_t canrecv_Handle;
TaskHandle_t init_Handle;

SemaphoreHandle_t bldc_sem;
SemaphoreHandle_t servo_sem;
SemaphoreHandle_t print_sem;

BaseType_t cap_Give;
BaseType_t print_Give;
BaseType_t can_Give;

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
/*    char buf[128];
    int buf_len;
*/
    etpwmInit();
    hetInit();
    hetRamInit();
    sciInit();
    etpwmStartTBCLK();

    print_sem=xSemaphoreCreateBinary();
    xSemaphoreGive(print_sem);


    if(xTaskCreate(init_task,"initalize",configMINIMAL_STACK_SIZE*2,NULL,3,&init_Handle)!=pdPASS){//인터럽트 관련 페리페럴
        for(;;)                                                                                   //초기화 태스크
            ;
    }


    if(xTaskCreate(motor_bldc,"bldc",configMINIMAL_STACK_SIZE*2,NULL,1,&bldc_Handle)!=pdPASS){//bldc 태스크
        for(;;)
            ;
    }

    if(xTaskCreate(motor_servo,"servo",configMINIMAL_STACK_SIZE*2,NULL,1,&servo_Handle)!=pdPASS){//servo 태스크
        for(;;)
            ;
    }

    if(xTaskCreate(pid,"pid",configMINIMAL_STACK_SIZE*2,NULL,1,&pid_Handle)!=pdPASS){//pid 태스크
        for(;;)
            ;
    }



    if(xTaskCreate(send_msg,"can_recv",configMINIMAL_STACK_SIZE*2,NULL,1,&canrecv_Handle)!=pdPASS){//can메시지 받았을때 반응
        for(;;)                                                                                    //하는 태스크
            ;
    }


    vTaskStartScheduler();//스케줄링 시작

/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
void init_task(void * pbParameters){
/*    char buf[64];
    int buf_len;
*/
    for(;;){
        init_back_flag=1;

        ecapInit();
        canInit();

/*
        sprintf(buf,"init complete\n\r\0");
        buf_len=strlen(buf);
        sci_display(sciREGx,buf,buf_len);
*/
        vTaskDelete(NULL);//초기화를 마친후 자기자신을 스케줄링에서 제거
/*
        sprintf(buf,"ERROR init task isn't deleted");
        buf_len=strlen(buf);
        sci_display(sciREGx,buf,buf_len);
 */   }
}

void pid(void* pbParameter){
    char buf[64];
    int buf_len;

    for(;;){
        if(ulTaskNotifyTake(pdTRUE,(TickType_t)10)){
          /*  sprintf(buf,"<<<<<<<< i'm pid <<<<<<\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
           */ xTaskNotifyGive(bldc_Handle);
            //wait_ms(0,1);
        }else{
         /*   sprintf(buf,"<<<<<<<<pid blocked<<<<<<\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
        */}
    }
}

void send_msg(void* pbParameter){
/*    char buf[64];
    int buf_len;
    int i=0;
*/
    for(;;){
        if(ulTaskNotifyTake(pdTRUE,(TickType_t)30)==1){
            can_tran[0]=(uint8)(throttle&0xFF);
            can_tran[1]=(uint8)((uint16)throttle>>8);
            can_tran[2]=(uint8)(rudder&0xFF);
            can_tran[3]=(uint8)((uint16)rudder>>8);

            canTransmit(canREG1,canMESSAGE_BOX1,&can_tran[0]);

           /* sprintf(buf,"//////////////recv msg from can:");
            buf_len=strlen(buf);
            for(i=0;i<DCNT;i++){
                sprintf(&buf[buf_len+i*3],"%2x ",can_recv[i]);
            }*/
/*
            sprintf(buf,"throttle:%d\t rudder:%d\n\r\0",throttle,rudder);
            buf_len=strlen(buf);
            sci_displayISR(sciREGx,buf,buf_len);
*/
           /* sprintf(&buf[buf_len+i*3],"//////////////\n\r\0");//4bit->1char => 8bit=>2char
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
           */ //vTaskDelay(1);
            //wait_ms(1,1);
        }else{
            /*sprintf(buf,"///////////can message not received for 1000 tick/////////\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);*/
        }
    }
}

void wait_ms(int num, int delay){//Het�쓣 �씠�슜�빐 ms�떒�쐞濡� �뵜�젅�씠瑜� 二쇨린 �쐞�빐 �궗�슜�븳 �븿�닔
    delay*=5000;//湲곕낯�씠 200ns �씠誘�濡� 5000�쓣 怨깊빐�꽌 1ms濡� 留욎텛�뼱以�
    hetRAM1->Instruction[num].Data=0;//0~delay源뚯� ��湲�
    while(((hetRAM1->Instruction[num].Data)>>7)<delay)//cnt�쓽 data�쓽 �븯�쐞 7bit�뒗 �벐吏� �븡�뒗 遺�遺꾩쑝濡� �긽�쐞 25bit濡쒕��꽣 媛믪쓣 �뼸�뼱�샂
        ;
}

void motor_servo(void* pbParameters)
{
 /*   char buf[64];
    int buf_len;
*/
    for(;;){
        if(ulTaskNotifyTake(pdTRUE,(TickType_t )0x01)==pdTRUE){

        //if(ulTaskNotifyTake(pdTRUE,(TickType_t)0x01)==pdPASS){
  /*          sprintf(buf,"********servo_Start**********\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
*/
            rudder=(int)(((float)rudder)*0.9);
            rudder=2700-rudder;//기기상 문제로 방향이 반대
            if(rudder>=1250&&rudder<1450){//중립조절
                rudder=1250;
            }
            if(rudder>=900&&rudder<=1800)
                etpwmREG1->CMPB=rudder;// 900 1250 1800(서보모터범위)
            wait_ms(1,100);
/*
            sprintf(buf,"**********servo_End rudder:%d*********\n\r\0",rudder);
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
*/
            ecapEnableInterrupt(ecapREG2,ecapInt_CEVT3);//서보모터 구동이 끝났으므로
        }else{                                          //interrupt를 활성화.
  /*          sprintf(buf,"servo Blocked\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
     */   }
    }
}

void motor_bldc(void* pbParameters)
{
 /*   char buf[64];
    int buf_len;
*/
    for(;;){
        if(ulTaskNotifyTake(pdTRUE,(TickType_t)0x01)==pdTRUE){//notify 받은 후 동작
            ecapDisableInterrupt(ecapREG1,ecapInt_CEVT3);// capInterrupt Disable
/*
            sprintf(buf,"bldc Start\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
*/
            if(throttle>=950&&throttle<1450){//후진하는 경우 처리
                if(init_back_flag){//처음 후진 할 시 처리
                    etpwmREG1->CMPA=1500;
                    wait_ms(0,100);
                    etpwmREG1->CMPA=1000;
                    wait_ms(0,100);
                    etpwmREG1->CMPA=1500;
                    wait_ms(0,100);
                    init_back_flag=0;
                    /*
                    sprintf(buf,"---------init back-----------\n\r\0");
                    buf_len=strlen(buf);
                    sci_display(sciREGx,buf,buf_len);
                    */
                }
            }else if(throttle>=1550&&throttle<=2000){
                init_back_flag=1;//전진한경우 플래그 재활성화.
            }else{
                throttle=1500;
            }
            etpwmREG1->CMPA=throttle;//pwm신호 보내기.
            wait_ms(0,100);
/*
            sprintf(buf,"bldc_task_end CMPA:%d\n\r\0",throttle);
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);
*/
            ecapEnableInterrupt(ecapREG1,ecapInt_CEVT3);//이제 모터구동이 끝나고 다음 신호를 받음.

        }else{
            /*
            sprintf(buf,"-------------bldc_task_blocked--------------\n\r\0");
            buf_len=strlen(buf);
            sci_display(sciREGx,buf,buf_len);*/
        }
    }

}

void sci_display(sciBASE_t *sci, char* buf, int len){
    taskENTER_CRITICAL();//태스크가 critical section에 들어감을 알림(인터럽트가 잠시 비활성화됨)
    while(len+1){
        if(xSemaphoreTake(print_sem,(TickType_t)0x01)==pdTRUE){//바이너리 세마포어처리
            while(len--){
                while((sci->FLR&0x4)==4);
                sciSendByte(sci,*buf++);
            }
            xSemaphoreGive(print_sem);//출력을 마친후 세마포어 반환
        }
    }
    taskEXIT_CRITICAL();//태스크가 critical section을 나감을 알림(인터럽트가 활성화)
}


void sci_displayISR(sciBASE_t *sci, char* buf, int len)
{
    while(len--){
        while((sci->FLR&0x4)==4);
        sciSendByte(sci,*buf++);
    }
}

void init_all(void){//珥덇린�솕 遺�遺꾩쓣 紐⑥븘�몦 �븿�닔
    etpwmInit();
    hetInit();
    hetRamInit();
    sciInit();
    _enable_interrupt_();
    etpwmStartTBCLK();
    canInit();
    ecapInit();
   // ecapStartCounter(ecapREG1); they are included in ecapInit
   // ecapStartCounter(ecapREG2);
   // ecapEnableCapture(ecapREG1);
   // ecapEnableCapture(ecapREG2);
    init_back_flag=1;
}

void hetRamInit(void)//hetRAM 珥덇린�솕
{
    hetREG1->GCR=(hetREG1->GCR)&0xFFFFFFFE;//HET off
    hetRAM1->Instruction[0].Program=0x00002C80U;//HETRAM 에 cnt 명령어 할당.
    hetRAM1->Instruction[0].Control=0x01FFFFFFU;
    hetRAM1->Instruction[0].Data=0xFFFFFF80U;

    hetRAM1->Instruction[1].Program=0x00000C80U;//hetRAM에 cnt 명령어 할당.
    hetRAM1->Instruction[1].Control=0x01FFFFFFU;
    hetRAM1->Instruction[1].Data=0xFFFFFF80U;
    hetREG1->GCR=(hetREG1->GCR)|0x00000001;//HET on
}


void ecapNotification(ecapBASE_t *ecap, uint16 flag)
{
    int cap[2];
    char buf[128];
    int buf_len;

    cap[0]=ecap->CAP1;
    cap[1]=ecap->CAP2;


    if(ecap==ecapREG1){
        ecapDisableInterrupt(ecapREG1,ecapInt_CEVT3);//모터 구동을 하기전에 interrupt
                                                     //재발을 방지.
       /* sprintf(buf,"cap1 Start\n\r\0");
        buf_len=strlen(buf);
        sci_displayISR(sciREGx,buf,buf_len);*/

        throttle=(cap[1]-cap[0])/VCLK3_FREQ;


        if(throttle<950){
            throttle=1500;
        }

        throttle=(throttle-1000)*0.4+1300;//throttle값을 1300~1700의 범위로 조절

        if(throttle>1450&&throttle<1550){//중립범위 조절.
            throttle=1500;
        }

      /*  sprintf(buf,"cap output throttle:%d\n\r\0",throttle);
        buf_len=strlen(buf);
        sci_displayISR(sciREGx,buf,buf_len);*/

        vTaskNotifyGiveFromISR(pid_Handle,&cap_Give);//모터를 구동하는 태스크에 통지.
    }else{
        ecapDisableInterrupt(ecapREG2,ecapInt_CEVT3);//�꽌蹂대え�꽣�� 踰덇컝�븘媛�硫� �떎�뻾�릺湲� �쐞�븿.
/*
        sprintf(buf,"cap2 Start\n\r\0");
        buf_len=strlen(buf);
        sci_displayISR(sciREGx,buf,buf_len);
*/
        rudder=(cap[1]-cap[0])/VCLK3_FREQ;
        if(rudder<950){//�떊�샇 �걡寃쇱쓣 �븣 以묐┰泥섎━
            rudder=1500;
        }
/*
        sprintf(buf,"cap2 output rudder:%d\n\r\0",rudder);
        buf_len=strlen(buf);
        sci_displayISR(sciREGx,buf,buf_len);
*/
        vTaskNotifyGiveFromISR(servo_Handle,&cap_Give);
    }
    portYIELD_FROM_ISR(cap_Give);//�씤�꽣�읇�듃 �꽌鍮꾩뒪 猷⑦떞�뿉�꽌 �꽭留덊룷�뼱 �궗�슜�떆 �씠嫄� �뜥二쇱뼱�빞 �븳�떎怨� �삁�젣 肄붾뱶�뿉 �엳湲몃옒 異붽�.
}

void canMessageNotification(canBASE_t *node, uint32 messageBox){
    char buf[64];
    int buf_len;
/*
    sprintf(buf,"<<<<<<<<<<<<can Start<<<<<<<<<<<<<<<\n\r\0");
    buf_len=strlen(buf);
    sci_displayISR(sciREGx,buf,buf_len);
*/
    if(node==canREG1){
        while(!canIsRxMessageArrived(canREG1,canMESSAGE_BOX2))
            ;
        canGetData(canREG1,canMESSAGE_BOX2,(uint8*)&can_recv[0]);
    }

  /*  can_tran[0]=(uint8)(etpwmREG1->CMPA&0xFF);
    can_tran[1]=(uint8)(etpwmREG1->CMPA>>8);
    can_tran[2]=(uint8)(etpwmREG1->CMPB&0xFF);
    can_tran[3]=(uint8)(etpwmREG1->CMPB>>8);

    canTransmit(canREG1,canMESSAGE_BOX1,&can_tran[0]);
*/

/*
    sprintf(buf,"<<<<<<<<<<<<can end<<<<<<<<<<<<<<<\n\r\0");
    buf_len=strlen(buf);
    sci_displayISR(sciREGx,buf,buf_len);
*/
    vTaskNotifyGiveFromISR(canrecv_Handle,&can_Give);//canrecv태스크를 깨움.
    //vTaskNotifyGiveFromISR(pid_Handle,&can_Give);
   // vTaskNotifyGiveFromISR(servo_Handle,&can_Give);
    portYIELD_FROM_ISR(can_Give);
}

void canIntDisable(canBASE_t *node){
    node->CTL=node->CTL&0xfffDfffD;
}
void canIntEnable(canBASE_t *node){
    node->CTL=node->CTL|0x00020002;
}




/* USER CODE END */
