// DT : PORTA PIN0       SCK : PORTA PIN1
#include "HL_system.h"
#include "HL_sys_common.h"
#include "HL_gio.h"
#include "HL_sci.h"
#include <stdio.h>
#include <string.h>

#define UART sciREG1

char buf[128]={0};

void wait(uint32 time){
    int i=0;
    for(i=0;i<time;i++)
        ;
}

void sci_display(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((UART->FLR & 0x4)==4)
            ;
            sciSendByte(UART, *text++);
    }
}

unsigned long read_count(void)
{
    unsigned long count=0;
    unsigned char i;
    gioSetBit(gioPORTA,0,1);
    gioSetBit(gioPORTA,1,0);
    while(gioGetBit(gioPORTA,0));

    wait(3);                        //   > 0.1us

    for(i=0;i<24;i++){
        gioSetBit(gioPORTA,1,1);
        count=count<<1;
        wait(28);                   //1us
        gioSetBit(gioPORTA,1,0);
        wait(28);
        if(gioGetBit(gioPORTA,0)) count++;
    }

    gioSetBit(gioPORTA,1,1);
    wait(28);
    //count=count^0x800000;
    gioSetBit(gioPORTA,1,0);
    wait(28);
    return(count);
}

int main(void)
{
    long data=0;
    gioInit();
    sciInit();
    gioSetDirection(gioPORTA,0x02);
    unsigned int buf_len;

    for(;;)
    {
        data=read_count();
        sprintf(buf,"hx711 adc result = %d\n\r\0",data);
        buf_len = strlen(buf);
        sci_display(UART, (uint8 *)buf, buf_len);

        wait(2800000);  //0.1s
    }
    return 0;
}
