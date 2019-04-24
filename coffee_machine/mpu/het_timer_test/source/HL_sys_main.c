#include "HL_sys_common.h"
#include "HL_system.h"


/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
/* Define Task Handles */

void wait_ms(int delay){
    delay*=5000;
    hetRAM1->Instruction[0].Data=0;
    while(((hetRAM1->Instruction[0].Data)>>7)<delay)
        ;
}

void main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    hetInit();
    //hetRamInit();
    //hetInit();
    /* Set high end timer GIO port hetPort pin direction to all output */
    gioSetDirection(hetPORT1, 0xFFFFFFFF);
    gioSetDirection(gioPORTB, 0xffffffff);

    for(;;){
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        wait_ms(1000);   // 1.5초? 2초?
        wait_ms(1000);
        wait_ms(1000);
        wait_ms(1000);
        wait_ms(1000);

        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        wait_ms(1000);
        wait_ms(1000);
        wait_ms(1000);
        wait_ms(1000);
        wait_ms(1000);
    }

/* USER CODE END */
}
