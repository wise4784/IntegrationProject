#include "HL_sys_core.h"
#include "HL_sys_vim.h"
#include "HL_system.h"
#include "HL_esm.h"
#include "HL_sys_common.h"
#include "HL_rti.h"
#include "HL_gio.h"
#include "HL_sci.h"
#include <stdio.h>
#include <string.h>


void _disable_all_(void);
void _gio_a5_on_(void);
int _print_cpsr_(void);
int _print_spsr_(void);

#define UART        sciREG1

//#define vimRAM_RTI ((vimRAM_t *)0xFFF8200CU)

uint32_t cnt;


void wait(uint32 delay)
{
    int i;

    for(i = 0; i < delay; i++)
        ;
}

void sci_display(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((UART->FLR & 0x4) == 4)
            ;
        sciSendByte(UART, *text++);
    }
}

void disp_set(char *str)
{
    char buf[128] = {0};
    unsigned int buf_len;

    sprintf(buf, str);
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *)buf, buf_len);

    wait(100000);
}

int main(void)
{
    int cpsr_data;
    int spsr_data;
    char txt_buf[256] = {0};
    unsigned int buf_len;

#if 0



    gioInit();



    gioSetDirection(gioPORTA, 0x00000020);



    gioSetPort(gioPORTA, 0x00000020);



#endif

    _gio_a5_on_();

#if 1
    sciInit();
    disp_set("sci config success!!!\n\r\0");
    rtiInit();
    rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
    _enable_interrupt_();
    rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);

    for(;;)
    {
        cpsr_data = _print_cpsr_();
        sprintf(txt_buf, "cpsr = 0x%x\n\r\0", cpsr_data);
        buf_len = strlen(txt_buf);
        sci_display(sciREG1, (uint8 *) txt_buf, buf_len);
        //wait(4000000);

        spsr_data = _print_spsr_();
        sprintf(txt_buf, "spsr = 0x%x\n\r\0", spsr_data);
        buf_len = strlen(txt_buf);
        sci_display(sciREG1, (uint8 *) txt_buf, buf_len);
        wait(10000000);

        if(cnt == 11)
        {
            int *addr;
            _disable_all_();
            vimREG->REQMASKSET0 = (uint32)((uint32)1U << 0U)
                                    | (uint32)((uint32)1U << 1U)
                                    | (uint32)((uint32)0U << 2U);

            vimREG->FIRQPR0 = (uint32)((uint32)SYS_FIQ << 0U)
                                | (uint32)((uint32)SYS_FIQ << 1U)
                                | (uint32)((uint32)SYS_IRQ << 2U);
            addr = 0xFFF8200CU;
            *addr = NULL;
        }
    }
#endif
    return 0;
}

void rtiNotification(rtiBASE_t *rtiREG, uint32 notification)
{
    cnt++;

    gioSetPort(gioPORTA, gioGetPort(gioPORTA) ^ 0x00000020);
}
