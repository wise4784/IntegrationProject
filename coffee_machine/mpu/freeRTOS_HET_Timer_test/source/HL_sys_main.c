#include "HL_sys_common.h"
#include "HL_system.h"
/* USER CODE BEGIN (1) */
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "os_queue.h"
#include "os_semphr.h"
#include "os_task.h"
#include "os_timer.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
/* Define Task Handles */
xTaskHandle xTask1Handle;
/* Task1 */
void vTask1(void *pvParameters)
{
    for(;;)
    {
        /* Taggle HET[1] with timer tick */
        gioSetBit(hetPORT1, 17, gioGetBit(hetPORT1, 17) ^ 1);
        gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        vTaskDelay(3000);
    }
}
/* USER CODE END */
/* USER CODE BEGIN (2) */
/* USER CODE END */
void main(void)
{
/* USER CODE BEGIN (3) */
    gioInit();
    /* Set high end timer GIO port hetPort pin direction to all output */
    gioSetDirection(hetPORT1, 0xFFFFFFFF);
    gioSetDirection(gioPORTB, 0xffffffff);
    /* Create Task 1 */
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }
    /* Start Scheduler */
    vTaskStartScheduler();
    /* Run forever */
    while(1);
/* USER CODE END */
}
