#include "HL_sys_common.h"
#include "HL_system.h"

#include "HL_sci.h"
#include "HL_gio.h"
#include "HL_het.h"
#include "HL_adc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "HL_reg_het.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "os_queue.h"
#include "os_semphr.h"
#include "os_task.h"
#include "os_timer.h"

#define remote_receive gioPORTA,7
#define top_drop gioPORTB,2
// ir
volatile char remote_now=33; //default : '!'
volatile int decode=0;
void decode_ir(void);

// step motor
volatile int step;

//weight sensor
volatile long weight,weight_zero;
// drop
volatile uint8 drop_before, drop_time_counter;
volatile uint8 drop_count=0,drop_time_avg=0;
volatile uint8 drop_time_array[5];
volatile uint8 drop_sens_timing=0;

//temperature
volatile uint32 value;
volatile float temp;
adcData_t adc_data[1];

// valuable valuables
uint8 time_gap_set=300;
double time_left=0;
volatile long cnt;

#define TSIZE 10
uint8 text[TSIZE]={'I','R',' ','N','O','W','=','X','\n','\r'};
char buf[256]={0};
unsigned int buf_len;
uint8 bitcount[122]={0};
uint8 letter[12]={0};

/* Define Task Handles */
xTaskHandle xTask1;
QueueHandle_t sem1=NULL;

void sci_display(sciBASE_t *sci, uint8 *txt, uint32 len)
{
    while(len--)
    {
        while((sciREG1->FLR & 0x4) == 4)
            ;
        sciSendByte(sciREG1, *txt++);
    }
}


void delay_us(uint32 time){
    int i=28*time;
    while(i--)
        ;
}

void delay_ms(uint32 time){
    int i=27778*time;
    while(i--)
        ;
}

void wait(uint32 delay)
{
    int i;
    for(i = 0; i < delay; i++)
        ;
}

void wait_66us(uint32 delay){
    delay*=5000;//50us
    hetRAM1->Instruction[0].Data=0; // hetResetTimestamp()와 상동
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}

void wait_us(uint32 delay){
    delay*=75;//us
    hetRAM1->Instruction[0].Data=0; // hetResetTimestamp()와 상동
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}

/*
void wait2_66us(uint32 delay){
    delay*=5000;//us
    hetRAM2->Instruction[0].Data=0; // hetResetTimestamp()와 상동
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}
*/
void lcd_command(unsigned char);
void lcd_char(unsigned char);
void lcd_string(char *);
void lcd_init(void);

void remote_data_calc(void);



/* Task1 */
void vTask1(void *pvParameters) // remote data calc + sci display   with semaphore
{
    for(;;)
    {
        if(xSemaphoreTake(sem1, (TickType_t)0x01)==pdTRUE){
/*
            sprintf(buf,"drop gap avg = %d ms \n\r",drop_time_avg*10);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *) buf, buf_len);
*/
            if(decode==1)
            {
                remote_data_calc();
            }
            //delay_4us(2);
            sprintf(buf,"remote data calc finished \n\r\0");
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);

            if(remote_now<10)
                remote_now+=48;
/* 문제점 : 여기 주석 처리부분은 활성화하면 꼭 맛탱이가 간다. task 루프 안 돌고 여기서 멈춘다. <- remote_now 비교해서 led켜는건잘됨
*/          /*
            sprintf(buf,"ir_now : %d\n\r\0",remote_now);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *)buf, buf_len);
            */
/*     위의 문제점 아래 3줄로 해결했다. -> uint8로 보내니까 먹통안되고 루프 잘 돈다.
*/
            text[7]=remote_now;
            sci_display(sciREG1, &text[0], TSIZE);
            wait_66us(10);

            if(remote_now==48)  // 숫자0 버튼 눌리면 USER_LED2 TOGGLE
            gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);

        }
        xSemaphoreGive(sem1);
        vTaskDelay(1000);
    }
}

// lcd_display
void vTask2(void *pvParameters){
    char data[5]={0};
    int num=0,i=0;
    for(;;)
    {
        if(xSemaphoreTake(sem1, (TickType_t)0x01)==pdTRUE){

        if((remote_now%2)==0){

        gioSetBit(gioPORTB,3,0);
        lcd_char(0x80);             //  first line
        lcd_string("T:");
        lcd_char((((int)time_left/10000)%10)+48);
        lcd_char((((int)time_left/1000)%10)+48);
        lcd_char((((int)time_left/100)%10)+48);
        lcd_char((((int)time_left/10)%10)+48);
        lcd_char(((int)time_left%10)+48);

        lcd_string(", Gap:");
        lcd_char((((time_gap_set)/100)%10)+48);
        lcd_string(".");
        lcd_char((((time_gap_set)/10)%10)+48);
        lcd_string("s");
    /*
        //디스플레이 표시 숫자
        num=cnt;

        data[0]=(num/10000)%10;
        data[1]=(num/1000)%10;
        data[2]=(num/100)%10;
        data[3]=(num/10)%10;
        data[4]=num%10;

        gioSetBit(gioPORTB,3,0);
        lcd_char(0x88);             //  first line. 0x8f : last word. 0x88 9번째
        gioSetBit(gioPORTB,3,1);    //rs=1distance_sensor_to_water_ground 17.5
        for(i=0;i<5;i++)
        {
            lcd_char(data[i]+48);
        }

        lcd_char(223);  //degree display
        //lcd_char(48);
        //lcd_char(49);
         *
         *
         */

        gioSetBit(gioPORTB,3,0);
        lcd_char(0xc0);             // second line
        lcd_string("on for ");
        lcd_char((((int)cnt/10000)%10)+48);
        lcd_char((((int)cnt/1000)%10)+48);
        lcd_char((((int)cnt/100)%10)+48);
        lcd_string("sec   ");

        }

        if((remote_now%2)==1){

            gioSetBit(gioPORTB,3,0);
            lcd_char(0x80);             //  first line
            lcd_string("water : ");

            lcd_char(0x88);             //  first line. 0x8f : last word. 0x88 9번째
            gioSetBit(gioPORTB,3,1);    //rs=1distance_sensor_to_water_ground 17.5
            for(i=0;i<1;i++)
            {
                lcd_char(((int)weight/2700000)+48);
                lcd_char(((int)weight%10)+48);
                lcd_string("  cm ");
            }

            gioSetBit(gioPORTB,3,0);
            lcd_char(0xc0);             // second line
            lcd_string("temperature: ");
            lcd_char(((int)temp/10)+48);
            lcd_char(((int)temp%10)+48);
            lcd_char(223);  //degree display
            lcd_string("C");

        }


        //gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1); //user_led_1_toggle
        step=200;   //step motor move.
        }
        xSemaphoreGive(sem1);
        vTaskDelay(1000);
        //vTaskDelay(1000);
    }
}



void main(void)
{
    gioInit();
    sciInit();
    adcInit();

    vSemaphoreCreateBinary(sem1);

    gioSetDirection(gioPORTA, 0b01100111);
    gioSetDirection(gioPORTB, 0b11001000);
    //gioSetDirection(hetPORT1,0b00110001001000001010100000000000);
    hetInit();

    gioEnableNotification(remote_receive);
    pwmEnableNotification(hetREG2,pwm0,pwmEND_OF_PERIOD);
    pwmEnableNotification(hetREG2,pwm1,pwmEND_OF_PERIOD);
    //pwmStart(hetRAM2,pwm0);
    _enable_IRQ_interrupt_();

    lcd_init();

    sprintf(buf,"Initializing Success \n\r");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *) buf, buf_len);

    //gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
    //wait_66us(3000);
    sprintf(buf,"User_led1_toggle\n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    /* Set high end timer GIO port hetPort pin direction to all output */

    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, NULL) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    if (xTaskCreate(vTask2,"Task2", configMINIMAL_STACK_SIZE, NULL, 1, NULL) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    /* Start Scheduler */
    vTaskStartScheduler();
    /* Run forever */
    while(1);
/* USER CODE END */
}

void gioNotification(gioPORT_t *port, uint32 bit)
{
   // _disable_IRQ_interrupt_();

    //gioSetBit(gioPORTB, 6, 1);
    if(decode==0){
        decode_ir();
    }

    //_enable_IRQ_interrupt_();
}
//void   pwmNotification(hetBASE_t * hetREG,uint32 pwm, uint32 notification);
void pwmNotification(hetBASE_t *port, uint32 pwm,uint32 notification){
    static uint32 time=0;
    static int stp=0;
    int i=0,j=0;
    uint8 drop_now=0;
    unsigned long count=0;

    switch(pwm) {

    case 0 :
        //j++;
        //if(j%2==0){
        //if(xSemaphoreTake(sem1, (TickType_t)0x01)==pdTRUE){
            gioSetBit(hetPORT1,18,1);
            gioSetBit(hetPORT1,2,0);
            while(gioGetBit(hetPORT1,18));

            wait(3);                        //   > 0.1us

            for(i=0;i<24;i++){
                gioSetBit(hetPORT1,2,1);
                count=count<<1;
                wait_us(1);                   //1us
                gioSetBit(hetPORT1,2,0);
                wait_us(1);
                if(gioGetBit(hetPORT1,18)) count++;
            }

            gioSetBit(hetPORT1,2,1);
            wait_us(1);
            count=count^0x800000;
            weight=count;
            gioSetBit(hetPORT1,2,0);
 //        }
 //        xSemaphoreGive(sem1);
         //vTaskDelay(6000);

         //temperature sensing
         adcStartConversion(adcREG1, adcGROUP1);
         while((adcIsConversionComplete(adcREG1, adcGROUP1)) == 0)
                     ;
         adcGetData(adcREG1, adcGROUP1, &adc_data[0]);
         value = adc_data[0].value;
         temp = (float)value/16383;
         temp = -66.875 + (float)(217.75*3.3*temp);

         // display sensed
         sprintf(buf,"weight calculated : %d, temperature : %f\n\r\0",count,temp);
         buf_len = strlen(buf);
         sci_display(sciREG1, (uint8 *)buf, buf_len);

         // user_led_toggle

         gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
        break;

    case 1 :
        time++;
        for(j=0;j<2;j++){

            drop_now=gioGetBit(top_drop);
            if((drop_before==1)&&(drop_now==0)){
                drop_count++;
                drop_time_array[drop_count%5]=time;
                if((drop_count%5)==4){
                    drop_time_avg=(drop_time_array[4]-drop_time_array[0])/4;

                }
            }
            drop_before=drop_now;
        }

        if(time%10==0){
            sprintf(buf,"drop_count : %d , drop gap avg = %d ms \n\r",drop_count, drop_time_avg*10);
            buf_len = strlen(buf);
            sci_display(sciREG1, (uint8 *) buf, buf_len);
        }

        if(step>0){
            if(stp>step){
                step=0;
                stp=0;

                sprintf(buf,"---------step motor finish \n\r\0");
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
            }
            stp++;
            switch(cnt) {
            case 0: gioSetBit(hetPORT1,12,1);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,16,0);
                    cnt=1;
                    break;
            case 1: gioSetBit(hetPORT1,12,1);
                    gioSetBit(hetPORT1,14,1);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,16,0);
                    cnt=2;
                    break;
            case 2: gioSetBit(hetPORT1,12,0);
                    gioSetBit(hetPORT1,14,1);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,16,0);
                    cnt=3;
                    break;
            case 3: gioSetBit(hetPORT1,12,0);
                    gioSetBit(hetPORT1,14,1);
                    gioSetBit(hetPORT1,30,1);
                    gioSetBit(hetPORT1,16,0);
                    cnt=4;
                    break;
            case 4: gioSetBit(hetPORT1,12,0);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,30,1);
                    gioSetBit(hetPORT1,16,0);
                    cnt=5;
                    break;
            case 5: gioSetBit(hetPORT1,12,0);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,30,1);
                    gioSetBit(hetPORT1,16,1);
                    cnt=6;
                    break;
            case 6: gioSetBit(hetPORT1,12,0);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,16,1);
                    cnt=7;
                    break;
            case 7: gioSetBit(hetPORT1,12,1);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,16,1);
                    cnt=0;
                    break;
            }
        }

        else if(step<0){
            if(stp<step){
                step=0;
                stp=0;

                sprintf(buf,"---------step motor finish \n\r\0");
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *)buf, buf_len);
            }
            stp--;
            switch(cnt) {
            case 0: gioSetBit(hetPORT1,16,1);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,12,0);
                    cnt=1;
                    break;
            case 1: gioSetBit(hetPORT1,16,1);
                    gioSetBit(hetPORT1,30,1);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,12,0);
                    cnt=2;
                    break;
            case 2: gioSetBit(hetPORT1,16,0);
                    gioSetBit(hetPORT1,30,1);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,12,0);
                    cnt=3;
                    break;
            case 3: gioSetBit(hetPORT1,16,0);
                    gioSetBit(hetPORT1,30,1);
                    gioSetBit(hetPORT1,14,1);
                    gioSetBit(hetPORT1,12,0);
                    cnt=4;
                    break;
            case 4: gioSetBit(hetPORT1,16,0);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,14,1);
                    gioSetBit(hetPORT1,12,0);
                    cnt=5;
                    break;
            case 5: gioSetBit(hetPORT1,16,0);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,14,1);
                    gioSetBit(hetPORT1,12,1);
                    cnt=6;
                    break;
            case 6: gioSetBit(hetPORT1,16,0);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,12,1);
                    cnt=7;
                    break;
            case 7: gioSetBit(hetPORT1,16,1);
                    gioSetBit(hetPORT1,30,0);
                    gioSetBit(hetPORT1,14,0);
                    gioSetBit(hetPORT1,12,1);
                    cnt=0;
                    break;
            }
        }
        break;
    }
}

void lcd_command(unsigned char cmmd){

    gioSetBit(gioPORTA,5,((cmmd>>7) & 0x01));
    gioSetBit(gioPORTA,2,((cmmd>>6) & 0x01));
    gioSetBit(gioPORTA,1,((cmmd>>5) & 0x01));
    gioSetBit(gioPORTA,0,((cmmd>>4) & 0x01));

    delay_us(1);
    gioSetBit(gioPORTA,6,1);
    delay_ms(1);
    gioSetBit(gioPORTA,6,0);

}

void lcd_char(unsigned char cmmd)
{
    gioSetBit(gioPORTA,5,((cmmd>>7) & 0x01));
    gioSetBit(gioPORTA,2,((cmmd>>6) & 0x01));
    gioSetBit(gioPORTA,1,((cmmd>>5) & 0x01));
    gioSetBit(gioPORTA,0,((cmmd>>4) & 0x01));


    gioSetBit(gioPORTA,6,1);
    delay_ms(1);
    gioSetBit(gioPORTA,6,0);

    delay_us(200);

    gioSetBit(gioPORTA,5,((cmmd>>3) & 0x01));
    gioSetBit(gioPORTA,2,((cmmd>>2) & 0x01));
    gioSetBit(gioPORTA,1,((cmmd>>1) & 0x01));
    gioSetBit(gioPORTA,0,(cmmd & 0x01));

    gioSetBit(gioPORTA,6,1);
    delay_ms(1);
    gioSetBit(gioPORTA,6,0);
    delay_ms(2);
}

void lcd_char_variable(unsigned char cmmd)
{
    gioSetBit(gioPORTA,5,((cmmd>>7) & 0x01));
    gioSetBit(gioPORTA,2,((cmmd>>6) & 0x01));
    gioSetBit(gioPORTA,1,((cmmd>>5) & 0x01));
    gioSetBit(gioPORTA,0,((cmmd>>4) & 0x01));


    gioSetBit(gioPORTA,6,1);
    delay_ms(1);
    gioSetBit(gioPORTA,6,0);

    delay_us(200);

    gioSetBit(gioPORTA,5,((cmmd>>3) & 0x01));
    gioSetBit(gioPORTA,2,((cmmd>>2) & 0x01));
    gioSetBit(gioPORTA,1,((cmmd>>1) & 0x01));
    gioSetBit(gioPORTA,0,(cmmd & 0x01));

    gioSetBit(gioPORTA,6,1);
    delay_ms(1);
    gioSetBit(gioPORTA,6,0);
    delay_ms(2);
}


void lcd_string(char *str)
{
    int i=0;
    gioSetBit(gioPORTB,3,1);    //rs=1

    for(i=0;str[i]!=0;i++)
    {
        lcd_char(str[i]);
    }
}

void lcd_init(void){

    int i=0;

    //rw=0, RS=0, E=0
    gioSetBit(gioPORTB,2,0);
    gioSetBit(gioPORTB,3,0);
    gioSetBit(gioPORTA,6,0);
    delay_ms(35);
    //func set
    for(i=0;i<2;i++)
    {
        lcd_command(0x30);
        delay_ms(1);
    }
    //lcd_command(0x30);
    //delay_us(150);
    //lcd_command(0x30);
    lcd_command(0x20);

    lcd_char(0x28);
    lcd_char(0x0c);
    lcd_char(0x01);
    lcd_char(0x06);
    lcd_char(0x80);

#if 0   //working. letter weird
    lcd_char(0x28);
    lcd_char(0x0f);     // display on off ctrl.
    lcd_char(0x01);     // clear display
    lcd_char(0x06);     // set cursor move direction and shift
    lcd_char(0x80);     // sets ddram address. ddram data is sent after this.
#endif

#if 0  // not working
    lcd_char(0x80);
    lcd_char(0x10);
    lcd_char(0x70);
#endif

    gioSetBit(gioPORTB,6, (0x00 & (0x10 >>4))); //bit moving is working
    delay_ms(5);
    lcd_string("hello");

    gioSetBit(gioPORTB,3,0);
    lcd_char(0xc0); // go to 2nd line
    lcd_string("HELLO LCD");
}


// IR REMOTE CONTROLLER CODE

void decode_ir(void){
    int i=0;
    //_disable_IRQ_interrupt_();

    while(gioGetBit(gioPORTA,7)==1){

    }

    for(i=0;i<122;i++)
    {
        //delay_4us(167);  //670us
        wait_us(562);

        if(gioGetBit(gioPORTA,7)==1)
            bitcount[i]=1+48;
        else
            bitcount[i]=0+48;
    }

    decode=1;

    sci_display(sciREG1,bitcount,122);
    wait(10);
}
void remote_data_calc()
{
    //gioSetPort(gioPORTB, gioGetPort(gioPORTB)^ 0x0000080);
    sprintf(buf,"access in remote_data_calc \n\r\0");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    int i=0;
    while(!((bitcount[i]==48)&&(bitcount[i+1]==49)&&(bitcount[i+2]==49)&&(bitcount[i+3]==49)&&(bitcount[i+4]==48)&&(bitcount[i+5]==49)&&(bitcount[i+6]==49)&&(bitcount[i+7]==49)&&(bitcount[i+8]==48)))
    {
        i++;
        if(i==50)
            break;
    }

/*
    sprintf(buf, "num = %d!\n\r\0",i);
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);
*/
    letter[0]+=bitcount[i+32]*pow(2,3);
    letter[0]+=bitcount[i+33]*pow(2,2);
    letter[0]+=bitcount[i+34]*pow(2,1);
    letter[0]+=bitcount[i+35]*pow(2,0);
    letter[1]+=bitcount[i+36]*pow(2,3);
    letter[1]+=bitcount[i+37]*pow(2,2);
    letter[1]+=bitcount[i+38]*pow(2,1);
    letter[1]+=bitcount[i+39]*pow(2,0);

    letter[2]+=bitcount[i+40]*pow(2,3);
    letter[2]+=bitcount[i+41]*pow(2,2);
    letter[2]+=bitcount[i+42]*pow(2,1);
    letter[2]+=bitcount[i+43]*pow(2,0);
    letter[3]+=bitcount[i+44]*pow(2,3);
    letter[3]+=bitcount[i+45]*pow(2,2);
    letter[3]+=bitcount[i+46]*pow(2,1);
    letter[3]+=bitcount[i+47]*pow(2,0);

    letter[4]+=bitcount[i+48]*pow(2,3);
    letter[4]+=bitcount[i+49]*pow(2,2);
    letter[4]+=bitcount[i+50]*pow(2,1);
    letter[4]+=bitcount[i+51]*pow(2,0);
    letter[5]+=bitcount[i+52]*pow(2,3);
    letter[5]+=bitcount[i+53]*pow(2,2);
    letter[5]+=bitcount[i+54]*pow(2,1);
    letter[5]+=bitcount[i+55]*pow(2,0);

    letter[6]+=bitcount[i+56]*pow(2,3);
    letter[6]+=bitcount[i+57]*pow(2,2);
    letter[6]+=bitcount[i+58]*pow(2,1);
    letter[6]+=bitcount[i+59]*pow(2,0);
    letter[7]+=bitcount[i+60]*pow(2,3);
    letter[7]+=bitcount[i+61]*pow(2,2);
    letter[7]+=bitcount[i+62]*pow(2,1);
    letter[7]+=bitcount[i+63]*pow(2,0);

    letter[8]+=bitcount[i+64]*pow(2,3);
     letter[8]+=bitcount[i+65]*pow(2,2);
     letter[8]+=bitcount[i+66]*pow(2,1);
     letter[8]+=bitcount[i+67]*pow(2,0);
     letter[9]+=bitcount[i+68]*pow(2,3);
     letter[9]+=bitcount[i+69]*pow(2,2);
     letter[9]+=bitcount[i+70]*pow(2,1);
     letter[9]+=bitcount[i+71]*pow(2,0);

     letter[10]+=bitcount[i+72]*pow(2,3);
      letter[10]+=bitcount[i+73]*pow(2,2);
      letter[10]+=bitcount[i+74]*pow(2,1);
      letter[10]+=bitcount[i+75]*pow(2,0);
      letter[11]+=bitcount[i+76]*pow(2,3);
      letter[11]+=bitcount[i+77]*pow(2,2);
      letter[11]+=bitcount[i+78]*pow(2,1);
      letter[11]+=bitcount[i+79]*pow(2,0);
/*
      sprintf(buf," result : %x %x %x %x %x %x %x %x %x %x %x %x !\n\r\0",letter[0],letter[1],letter[2],letter[3],letter[4],letter[5],letter[6],letter[7],letter[8],letter[9],letter[10],letter[11]);
      buf_len = strlen(buf);
      sci_display(sciREG1, (uint8 *)buf, buf_len);
*/
    switch (letter[0]){
    case 0xd7 :

        switch (letter[1])
        {
            case 0xd7 :

                if(letter[5]==0xd5) remote_now=7;
                else                remote_now=3;
                if(letter[2]==0xd5) remote_now=6;
            break;

            case 0xd5 :

                if(letter[2]==0xdd)
                {
                    switch (letter[3])
                    {
                        case 0xdd : remote_now='h';   // sharp
                        break;
                        case 0xd5 : remote_now=1;
                        break;
                        case 0xd7 : remote_now=8;
                        break;
                    }
                }
                else
                {
                    switch (letter[3])
                    {
                        case 0xd5 : remote_now=9;
                        break;
                        case 0xd7 : remote_now=0;
                        break;
                    }
                }
            break;
        }
    break;


    case 0xd5 :
        switch (letter[1])
        {
            case 0xdd :
                switch(letter[2])
                {   case 0xdd :
                        if(letter[3]==0xd7)
                                    remote_now='s';   //s=star;
                        else        remote_now=2;
                    break;
                    case 0xd7 :     remote_now='r';   //r=right
                    break;
                    case 0xd5 :     remote_now='d';   //d=down
                    break;
                }
            break;

            case 0xd7 :
                if(letter[2]==0xd7) remote_now='k';   //o=ok
                else                remote_now=4;
            break;

            case 0xd5 :
                switch (letter[3])
                {
                    case 0xdd :     remote_now='u';   //u= up
                    break;
                    case 0xd5 :     remote_now='l';   //l= left
                    break;
                    case 0xd7 :     remote_now=5;
                    break;
                }
            break;
        }
    }


/*
    fal=0;
      for(i=0;i<12;i++)
      {
          if(letter[i]!=letterok[i])
              fal++;
      }

      if(fal<=1){
          gioSetPort(gioPORTB, gioGetPort(gioPORTB)^ 0x0000040);
          remote_now++;
      }
*/
      i=0;
      for(i=0;i<12;i++)
          letter[i]=0;

      decode=0;
}



// het interrrupt

#if 0
void pwmEnableNotification(hetBASE_t * hetREG, uint32 pwm, uint32 notification)
{
    hetREG->FLG     = notification << (pwm << 1U);
    hetREG->INTENAS = notification << (pwm << 1U);
}



/** @fn void pwmDisableNotification(hetBASE_t * hetREG, uint32 pwm, uint32 notification)
*   @brief Enable pwm notification
*   @param[in] hetREG Pointer to HET Module:
*              - hetREG1: HET1 Module pointer
*              - hetREG2: HET2 Module pointer
*   @param[in] pwm Pwm signal:
*              - pwm0: Pwm 0
*              - pwm1: Pwm 1
*              - pwm2: Pwm 2
*              - pwm3: Pwm 3
*              - pwm4: Pwm 4
*              - pwm5: Pwm 5
*              - pwm6: Pwm 6
*              - pwm7: Pwm 7
*   @param[in] notification Pwm notification:
*              - pwmEND_OF_DUTY:   Notification on end of duty
*              - pwmEND_OF_PERIOD: Notification on end of end period
*              - pwmEND_OF_BOTH:   Notification on end of both duty and period
*/
/* SourceId : HET_SourceId_008 */
/* DesignId : HET_DesignId_008 */
/* Requirements : HL_CONQ_HET_SR17 */
#endif

