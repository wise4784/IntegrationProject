/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* (c) Texas Instruments 2009-2013, All rights reserved. */

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_sys_core.h"

/* USER CODE BEGIN (1) */
#include "HL_gio.h"
#include "HL_reg_het.h"
#include "HL_rti.h"
#include "HL_reg_rti.h"
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

void main(void)
{
/* USER CODE BEGIN (3) */
	gioInit();

	gioSetDirection(hetPORT1, (1ull << 0) | (1ull << 31)); /* Enable N2HET1_00 and N2HET1_31 output buffer */

	gioSetBit(hetPORT1, 31ull, 0ull); /* Switch off LED D4 */
	gioSetBit(hetPORT1,  0ull, 0ull); /* Switch off LED D5 */

	rtiInit();

	rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE0);
	rtiEnableNotification(rtiREG1, rtiNOTIFICATION_COMPARE1);

	rtiStartCounter(rtiREG1, rtiCOUNTER_BLOCK0);

	_enable_IRQ();

	while(1);
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
