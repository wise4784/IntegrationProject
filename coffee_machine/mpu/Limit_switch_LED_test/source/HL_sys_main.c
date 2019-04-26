
#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_het.h"
#include <stdio.h>


/* USER CODE BEGIN (1) */
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/* USER CODE END */


void sci_display_txt(sciBASE_t *sci, uint8 *txt, uint32 len)
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

int main(void)
{
    uint8 value=0;

    gioSetDirection(gioPORTB,0xffffff);
    gioSetDirection(hetPORT1,0x000000);
    gioInit();
    sciInit();


    for(;;){
    wait(5000000);
    value=(uint8)gioGetBit(hetPORT1,12)+48U;
    sci_display_txt(sciREG1, &value,1);
    }

/* USER CODE BEGIN (3) */
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
