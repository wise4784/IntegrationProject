#include "HL_sys_common.h"
//#include "sys_core.h"
#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_rti.h"
#include "HL_system.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "HL_reg_het.h"
#include "HL_het.h"

#define remote_receive gioPORTA,7


char remote_now=33;
volatile int decode=0;
int fal=0;
void decode_ir(void);

char buf[256]={0};
unsigned int buf_len;



uint8 bitcount[122]={0};
uint8 letter[12]={0};
uint8 letter1[12]={0xd7,0xd5,0xdD,0xd5,0xd7,0xd5,0xd7,0xd5,0xdD,0xdD,0xdD,0xd7};
uint8 letter2[12]={0xd5,0xdd,0xdd,0xd5,0xd7,0xd5,0xdd,0xd5,0xdD,0xdD,0xdD,0xd7};
uint8 letter3[12]={0xd7,0xd7,0xd7,0xd5,0xd5,0xdd,0xd5,0xd5,0xdD,0xdD,0xdD,0xd7};
uint8 letter4[12]={0xd5,0xd7,0xd5,0xd5,0xdd,0xd7,0xd7,0xd5,0xdD,0xdD,0xdD,0xd7};
uint8 letter5[12]={0xd5,0xd5,0xd5,0xd7,0xd5,0xdd,0xdd,0xdd,0xdD,0xdD,0xdD,0xd7};
uint8 letter6[12]={0xd7,0xd7,0xd5,0xd5,0xd7,0xd5,0xd5,0xdd,0xdD,0xdD,0xdD,0xd7};
uint8 letter7[12]={0xd7,0xd7,0xd7,0xd5,0xd5,0xd5,0xd5,0xd7,0xd7,0xd7,0xd7,0xd7};
uint8 letter8[12]={0xd7,0xd5,0xdD,0xd7,0xd5,0xd5,0xd7,0xd5,0xdD,0xd7,0xd7,0xd7};
uint8 letter9[12]={0xd7,0xd5,0xd7,0xd5,0xd5,0xd5,0xdd,0xdd,0xd7,0xd7,0xd7,0xd7};
uint8 letter0[12]={0xd7,0xd5,0xd7,0xd7,0xd5,0xd5,0xd7,0xd7,0xd5,0xd7,0xd7,0xd7};
uint8 letterstar[12]={0xd5,0xdd,0xdD,0xd7,0xd5,0xd5,0xdd,0xd5,0xdD,0xd7,0xd7,0xd7};
uint8 lettersharp[12]={0xd7,0xd5,0xdD,0xdd,0xd5,0xd5,0xd7,0xd5,0xd7,0xd7,0xd7,0xd7};
uint8 letterok[12]={0xd5,0xd7,0xd7,0xd7,0xd5,0xd5,0xdd,0xdd,0xd5,0xd7,0xd7,0xd7};
uint8 letterleft[12]={0xd5,0xd5,0xdD,0xd5,0xd5,0xdd,0xdd,0xdd,0xd7,0xd7,0xd7,0xd7};
uint8 letterright[12]={0xd5,0xdd,0xd7,0xd7,0xd5,0xd,0xd7,0xd5,0xdD,0xd5,0xdD,0xd7};
uint8 letterup[12]={0xd5,0xd5,0xdD,0xdd,0xd5,0xd7,0xd7,0xd7,0xd5,0xd7,0xd7,0xd7};
uint8 letterdown[12]={0xd5,0xdd,0xd5,0xdd,0xd7,0xd5,0xdd,0xd7,0xd7,0xd5,0xdD,0xd7};



void sci_display(sciBASE_t *sci, uint8 *txt, uint32 len)
{
    while(len--)
    {
        while((sciREG1->FLR & 0x4) == 4)
            ;
        sciSendByte(sciREG1, *txt++);
    }
}


void wait(uint32 delay)
{
    int i;
    for(i = 0; i < delay; i++)
        ;
}

/*
void delay_us(uint32 time){
    int i=28*time;
    while(i--)
        ;
}
*/

void delay_us(uint32 delay){
    delay*=75;//us
    hetRAM1->Instruction[0].Data=0; // hetResetTimestamp()와 상동
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}

void delay_ms(uint32 time){
    int i=27778*time;
    while(i--)
        ;
}



void remote_data_calc()
{
    gioSetPort(gioPORTB, gioGetPort(gioPORTB)^ 0x0000080);
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

      i=0;
      for(i=0;i<12;i++)
          letter[i]=0;
      fal=0;
      decode=0;
}



int main(void)
{

    gioInit();
    sciInit();
    hetInit();
//    rtiInit();
    gioSetDirection(gioPORTA, 0x00);
    gioSetDirection(gioPORTB, 0b11000000);
//    gioSetDirection(hetPORT1, 0b0101000000000000000100000000);

//    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
    gioEnableNotification(remote_receive);
    _enable_IRQ_interrupt_();
//    rtiStartCounter(rtiREG1,rtiCOUNTER_BLOCK0);


    for(;;)
    {
        if(decode==1)
        {
            remote_data_calc();

        }
        delay_us(10);

        if(remote_now<10)
            remote_now+=48;
        sprintf(buf,"ir_now : %c\n\r\0",remote_now);
        buf_len = strlen(buf);
        sci_display(sciREG1, (uint8 *)buf, buf_len);

    }
    return 0;
}

void decode_ir(void){
    int i=0;
    //_disable_IRQ_interrupt_();

    while(gioGetBit(gioPORTA,7)==1){

    }

    for(i=0;i<122;i++)
    {
        delay_us(562);// het 사용시 562~563이 인식 제일 잘 됨.-> ir신호 562.5us와 칼같이 맞네.. 단)ok버튼은 l버튼과 같게 나옴.

        if(gioGetBit(gioPORTA,7)==1)
            bitcount[i]=1+48;
        else
            bitcount[i]=0+48;
    }

    decode=1;

    sci_display(sciREG1,bitcount,122);
    delay_us(10);
}

void gioNotification(gioPORT_t *port, uint32 bit)
{
    _disable_IRQ_interrupt_();

    //gioSetBit(gioPORTB, 6, 1);
    if(decode==0){
        decode_ir();
    }

    //_enable_IRQ_interrupt_();
}

