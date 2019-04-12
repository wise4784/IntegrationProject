/** @file HL_notification.c 
*   @brief User Notification Definition File
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file  defines  empty  notification  routines to avoid
*   linker errors, Driver expects user to define the notification. 
*   The user needs to either remove this file and use their custom 
*   notification function or place their code sequence in this file 
*   between the provided USER CODE BEGIN and USER CODE END.
*
*/

/* Include Files */

#include "HL_esm.h"
#include "HL_sci.h"
#include "HL_epc.h"
#include "HL_emac.h" 
#include "HL_sys_dma.h"

/* USER CODE BEGIN (0) */
/* USER CODE END */
#pragma WEAK(esmGroup1Notification)
void esmGroup1Notification(esmBASE_t *esm, uint32 channel)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (1) */
/* USER CODE END */
}

/* USER CODE BEGIN (2) */
/* USER CODE END */
#pragma WEAK(esmGroup2Notification)
void esmGroup2Notification(esmBASE_t *esm, uint32 channel)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (3) */
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
#pragma WEAK(esmGroup3Notification)
void esmGroup3Notification(esmBASE_t *esm, uint32 channel)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (5) */
/* USER CODE END */
    for(;;)
    { 
    }/* Wait */  
/* USER CODE BEGIN (6) */
/* USER CODE END */
}

/* USER CODE BEGIN (7) */
/* USER CODE END */

#pragma WEAK(dmaGroupANotification)
void dmaGroupANotification(dmaInterrupt_t inttype, uint32 channel)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (8) */
/* USER CODE END */
}

/* USER CODE BEGIN (9) */
/* USER CODE END */

/* USER CODE BEGIN (10) */
/* USER CODE END */

/* USER CODE BEGIN (11) */
/* USER CODE END */

#pragma WEAK(sciNotification)
void sciNotification(sciBASE_t *sci, uint32 flags)     
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (32) */
/* USER CODE END */
}

/* USER CODE BEGIN (33) */
/* USER CODE END */



/* USER CODE BEGIN (46) */
/* USER CODE END */


/* USER CODE BEGIN (50) */
/* USER CODE END */


/* USER CODE BEGIN (53) */
/* USER CODE END */


/* USER CODE BEGIN (56) */
/* USER CODE END */

#pragma WEAK(epcCAMOverflowNotification)
void epcCAMOverflowNotification(void)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (57) */
/* USER CODE END */
}
#pragma WEAK(epcFIFOOverflowNotification)
void epcFIFOOverflowNotification(uint32 epcFIFOStatus)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (58) */
/* USER CODE END */
}

/* USER CODE BEGIN (59) */
/* USER CODE END */

#pragma WEAK(emacTxNotification)
void emacTxNotification(hdkif_t *hdkif)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (60) */
/* USER CODE END */
}

/* USER CODE BEGIN (61) */
/* USER CODE END */
#pragma WEAK(emacRxNotification)
void emacRxNotification(hdkif_t *hdkif)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (62) */
/* USER CODE END */
}

/* USER CODE BEGIN (63) */
/* USER CODE END */
