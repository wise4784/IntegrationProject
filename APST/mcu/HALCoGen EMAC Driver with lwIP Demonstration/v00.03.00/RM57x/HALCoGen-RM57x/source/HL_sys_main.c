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

/* USER CODE BEGIN (1) */
extern void EMAC_LwIP_Main (uint8_t * emacAddress);
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

uint8	emacAddress[6U] = 	{0x00U, 0x08U, 0xEEU, 0x03U, 0xA6U, 0x6CU};
uint32 	emacPhyAddress	=	1U;

void main(void)
{
/* USER CODE BEGIN (3) */
	EMAC_LwIP_Main(emacAddress);
/* USER CODE END */
}

/* USER CODE BEGIN (4) */
/* USER CODE END */
