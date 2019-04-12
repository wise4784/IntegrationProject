/** @file HL_errata.c
*   @brief Errata workaround Header File
*   @date 20.May.2014
*   @version 04.00.00
*
*   This file contains:
*   - Errata workaround API's
*/

/* (c) Texas Instruments 2009-2014, All rights reserved. */

#ifndef __HL_errata_H__
#define __HL_errata_H__

#include "HL_reg_pbist.h"
#include "HL_reg_system.h"

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN (0) */
/* USER CODE END */

#define PBISTERRATA_FAIL1         1U
#define PBISTERRATA_FAIL2         2U
#define PBISTERRATA_FAIL3         3U

void errata_PBIST_4(void);

void errataFailNotification(uint32 flag);
/* USER CODE BEGIN (1) */
/* USER CODE END */
#ifdef __cplusplus
}
#endif

#endif
