#include "HL_rti.h"
#include "HL_gio.h"
#include "HL_het.h"
#include "HL_sci.h"
#include "HL_system.h"
#include "HL_sys_common.h"

uint8 cnt=0;
uint32 total=0;

int main(void)
{

    //hetInit();
    gioSetDirection(gioPORTA,0x0ff);
    //gioSetDirection(hetPORT1,0x0ff);

    hetInit();

    gioInit();

    gioSetPort(hetPORT1,0x00);

    rtiInit();
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
    _enable_IRQ_interrupt_();
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);

    for(;;)
        ;

    return 0;
}

void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
    gioSetBit(hetPORT1,1,1);
    gioSetBit(hetPORT1,24,1);
    gioSetBit(hetPORT1,26,1);
    gioSetBit(hetPORT1,8,1);
    gioSetBit(hetPORT1,12,1);
#if 0
    total++;
    gioSetBit(gioPORTB,6,1);

    if(total>=1024)     // 반 바퀴 돌았다.
    {
        gioSetBit(gioPORTB,6,0);
        cnt=9;
    }
    else{
        gioSetBit(gioPORTB,6,1);
    }

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
#endif
}

