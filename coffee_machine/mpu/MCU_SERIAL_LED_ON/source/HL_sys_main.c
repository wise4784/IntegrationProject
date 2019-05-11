#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_sci.h"
#include "HL_gio.h"
#include <string.h>
#include <stdio.h>

#define UART sciREG1
#define TSIZE   7

char txt_buf[100];
uint8 buf_len;
uint32 tmp[6];
char ins[TSIZE] = {'l', 'e', 'd', 'o', 'n', '\n', '\r'};

int tb_len;

void wait(uint32);

void sciSendText(sciBASE_t * , uint8*, uint32);

int main(void)
{
    gioInit();
    wait(10000);
    sciInit();
    wait(10000);
    gioSetDirection(gioPORTA, 0xffffffff);
    gioSetBit(gioPORTA, 0, 1);
    gioSetBit(gioPORTA, 1, 1);
    gioSetBit(gioPORTA, 2, 1);

    sprintf(txt_buf, "enter 1digit number.. \n\r\0");
    buf_len = strlen(txt_buf);
    sciSendText(sciREG1, (uint8 *)txt_buf, buf_len);

    for(;;)
    {
        tmp[0] = sciReceiveByte(sciREG1);
        if((tmp[0]>58)||(tmp[0]<48))
          {
              sprintf(txt_buf, "number please.. \n\r\0");
              buf_len = strlen(txt_buf);
              sciSendText(sciREG1, (uint8 *)txt_buf, buf_len);

          }
        sciSendText(sciREG1, &tmp[0], TSIZE);

        if(tmp[0]=='\n'||(tmp[0]<48))
        ;
        else
//            tmp[0] = tmp[0] - 48;
            ;

            switch(tmp[0])c
            {
            case '0':
                gioSetBit(gioPORTA, 0, 1);
                gioSetBit(gioPORTA, 1, 1);
                gioSetBit(gioPORTA, 2, 1);

                gioSetBit(gioPORTA, 0, 0);
                //gioSetBit(gioPORTA, 1, 0);
                //gioSetBit(gioPORTA, 2, 0);
                break;
            case '1':
                gioSetBit(gioPORTA, 0, 1);
                gioSetBit(gioPORTA, 1, 1);
                gioSetBit(gioPORTA, 2, 1);
                //gioSetBit(gioPORTA, 0, 0);
                //gioSetBit(gioPORTA, 1, 0);
                gioSetBit(gioPORTA, 1, 0);
                break;
            case '2':
                gioSetBit(gioPORTA, 0, 1);
                gioSetBit(gioPORTA, 1, 1);
                gioSetBit(gioPORTA, 2, 1);
                //gioSetBit(gioPORTA, 0, 0);
                //gioSetBit(gioPORTA, 1, 0);
                gioSetBit(gioPORTA, 2, 0);
                break;
            }

        wait(33333);
    }
    return 0;
}

void wait(uint32 delay)
{
    int i;
    for(i=0; i<delay; i++)
        ;
}

void sciSendText(sciBASE_t* sci, uint8 * text, uint32 len)

{
    while(len--)
    {
        while((sci->FLR & 0x04)==4)
            ;
        sciSendByte(sci, *text++);
    }
}
