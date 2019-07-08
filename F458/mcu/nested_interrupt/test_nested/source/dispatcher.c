
#include "HL_sys_common.h"
#include "HL_sys_vim.h"

typedef volatile struct vimRAM{
    t_isrFuncPTR ISR[VIM_CHANNELS+1];
}vimRAM_t;

static const vimRAM_t * const vimRAM = ((vimRAM_t *)0xFFF82000U);

void c_dispatch(void)
{
    int interruptindex=vimREG->IRQINDEX;
    t_isrFuncPTR handler=vimRAM->ISR[interruptindex];
    (*handler)();
}




