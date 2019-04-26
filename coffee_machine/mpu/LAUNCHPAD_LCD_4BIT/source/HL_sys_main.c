#include "HL_sys_common.h"
#include "HL_gio.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void lcd_string_xy(char row, char pos, char *str)
{
    if(row==0 && pos<16)
        lcd_command((pos & 0x0F)|0x80); // command first row
    else if(row==1 && pos<16)
        lcd_command((pos & 0x0f)|0xc0);
    lcd_string(str);
}

void lcd_clear()
{
    lcd_command(0x01);
    delay_ms(2);
    lcd_command(0x80);
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
}

void lcd_right_sh(){
    gioSetBit(gioPORTB,3,0);    //rs=0
    lcd_char(0x1C);
    delay_ms(1);
}

void lcd_left_sh(){
    gioSetBit(gioPORTB,3,0);    //rs=0
    lcd_char(0x18);
    delay_ms(1);
}

int main(void)
{
    gioInit();
    char data[5]={0};
    int num=0,i;
    gioSetDirection(gioPORTA, 0xffffffff);
    gioSetDirection(gioPORTB, 0xffffffff);
    gioSetBit(gioPORTB,6, (0x01 & (0x10 >>4))); //bit moving is working
    lcd_init();

    lcd_string("hello");

    gioSetBit(gioPORTB,3,0);
    lcd_char(0xc0); // go to 2nd line
    lcd_string("HELLO LCD");

    for(;;)
    {
        gioSetBit(gioPORTB,3,0);
        lcd_char(0x80);             //  first line
        lcd_string("value : ");

        gioSetBit(gioPORTB,3,0);
        lcd_char(0x88);
        num=77777;

        gioSetBit(gioPORTB,3,1);    //rs=1

        data[0]=(num/10000)%10;
        data[1]=(num/1000)%10;
        data[2]=(num/100)%10;
        data[3]=(num/10)%10;
        data[4]=num%10;

        lcd_char(0x88);             //  first line. 0x8f : last word. 0x88 9번째
        printf("%d",data[0]);
        gioSetBit(gioPORTB,3,1);    //rs=1
        for(i=0;i<5;i++)
        {
            lcd_char(data[i]+48);
        }


        //lcd_char(48);
        //lcd_char(49);

        gioSetBit(gioPORTB,3,0);
        lcd_char(0xc0);             // second line
        lcd_string("HELLO LCD");
        delay_ms(1);


    }


    return 0;
}
