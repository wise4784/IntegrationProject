#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
/* Include FreeRTOS scheduler files */
#include "FreeRTOS.h"
#include "os_task.h"
#include "os_semphr.h"

/* Include HET header file - types, definitions and function declarations for system driver */
#include "HL_het.h"
#include "HL_gio.h"
#include "HL_esm.h"

#include "HL_etpwm.h"

#include "lwip/err.h"
#include "lwip/pbuf.h"
#include "ipv4/lwip/ip_addr.h"
#include "lwip/udp.h"

#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void vTask1(void *pvParameters);
extern void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void udpTask(void *pvParameters);
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName );
extern void EMAC_LwIP_Main (uint8_t * emacAddress);

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

SemaphoreHandle_t xSemaphore = NULL;

/* Define Task Handles */
xTaskHandle xTask1Handle;
xTaskHandle xTask2Handle;
xTaskHandle xTask3Handle;
xTaskHandle xTask4Handle;
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

uint8	emacAddress[6U] = 	{0x00U, 0x08U, 0xeeU, 0x03U, 0xa6U, 0x6cU};
uint32 	emacPhyAddress	=	1U;

int main(void)
{
/* USER CODE BEGIN (3) */
    /*clear the ESM error manually*/
    esmREG->SR1[2] = 0xFFFFFFFFU;
    esmREG->SSR2   = 0xFFFFFFFF;
    esmREG->EKR = 0x0000000A;
    esmREG->EKR = 0x00000000;

    xSemaphore = xSemaphoreCreateBinary();

    if(xSemaphore == NULL)
    {
        return -1;
    }

    /* Set high end timer GIO port hetPort pin direction to all output */
    gioInit();
    gioSetDirection(gioPORTA, 0xFFFF);
    gioSetDirection(gioPORTB, 0xFFFF);

    /* Set high end timer GIO port hetPort pin direction to all output */
    //gioSetDirection(hetPORT1, 0xFFFFFFFF); //HDK uses NHET for LEDs

    //EMAC_LwIP_Main(emacAddress);
    EMAC_LwIP_Main(emacAddress);

    /* Create Task 1 */
    //    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle) != pdTRUE)
    if (xTaskCreate(vTask1,"Task1", configMINIMAL_STACK_SIZE, NULL,  ((configMAX_PRIORITIES-1)|portPRIVILEGE_BIT), &xTask1Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

    /* Create Task 2 */
    if (xTaskCreate(vTask2,"Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle) != pdTRUE)
    {
        /* Task could not be created */
        while(1);
    }

#if 0
    if(xTaskCreate(vTask3, "PWM", configMINIMAL_STACK_SIZE, NULL, 2, &xTask3Handle) != pdTRUE)
    {
        while(1);
    }
#endif

#if 1
    if(xTaskCreate(udpTask, "udp", 8 * configMINIMAL_STACK_SIZE, NULL, 3, &xTask4Handle) != pdTRUE)
    {
        while(1);
    }
#endif

    /* Start Scheduler */
    vTaskStartScheduler();

    /* Run forever */
    while(1);
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* Task1 */
void vTask1(void *pvParameters)
{
#if 1
    for(;;)
    {
        taskENTER_CRITICAL();

#if 0
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
#endif
            /* Taggle GIOB[6] with timer tick */
            //gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
#if 0
            xSemaphoreGive(xSemaphore);
        }
#endif

        taskEXIT_CRITICAL();

        vTaskDelay(300);
    }
#endif
}

void vTask2(void *pvParameters)
{
    for(;;)
    {
        taskENTER_CRITICAL();

#if 0
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
#endif
        /* Taggle GIOB[7] with timer tick */
            //gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
#if 0
            xSemaphoreGive(xSemaphore);
        }
#endif

        taskEXIT_CRITICAL();
        //gioSetBit(hetPORT1, 18, gioGetBit(hetPORT1, 18) ^ 1);  //LED on HDK, bottom
        vTaskDelay(500);
    }
}

#if 0
void vTask3(void *pvParameters)
{
    etpwmInit();

    for(;;)
    {
        xSemaphoreTake(xSemaphore, (TickType_t) 10);
        if(xSemaphore)
        {
            gioSetBit(gioPORTA, 0, gioGetBit(gioPORTA, 0) ^ 1);
            xSemaphoreGive(xSemaphore);
        }
        vTaskDelay(120);
    }
}
#endif

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    gioSetBit(gioPORTB, 7, 1);
    if (p != NULL)
    {
        gioSetBit(gioPORTB, 6, 1);
        //udp_sendto(pcb, p, IP_ADDR_BROADCAST, 7767); //dest port
        pbuf_free(p);
    }
}

void udpTask(void *pvParameters)
{
#if 0
    struct tcp_pcb *pcb;
    err_t err;

    pcb = tcp_new();
    LWIP_ASSERT(("httpd_init: tcp_new failed"), pcb != NULL);
    err = tcp_bind(pcb, IP_ADDR_ANY, HTTPD_SERVER_PORT);
    LWIP_ASSERT(("httpd_init: tcp_bind failed: %s", lwip_strerr(err)), err == ERR_OK);
    pcb = tcp_listen(pcb);
    LWIP_ASSERT(("httpd_init: tcp_listen failed"), pcb != NULL);
    /* initialize callback arg and accept callback */
    tcp_arg(pcb, pcb);
    tcp_accept(pcb, http_accept);
#endif
    struct udp_pcb *pcb;
    char msg[] = "udp test test test\r\n";
    struct pbuf *p;
    err_t err;

#if 0
    xSemaphoreTake(xSemaphore, (TickType_t) 10);
    if(xSemaphore)
    {
        pcb = udp_new();
        err = udp_bind(pcb, IP_ADDR_ANY, 7777);
        udp_recv(pcb, udp_echo_recv, NULL);
        xSemaphoreGive(xSemaphore);
    }
#else
    pcb = udp_new();
    udp_bind(pcb, IP_ADDR_ANY, 7767);
    udp_recv(pcb, udp_echo_recv, NULL);

#endif

    for(;;)
    {
        taskENTER_CRITICAL();

        p = pbuf_alloc(PBUF_TRANSPORT, sizeof(msg), PBUF_RAM);
        memcpy(p->payload, msg, sizeof(msg));
        udp_sendto(pcb, p, IP_ADDR_BROADCAST, 7767);
        pbuf_free(p);

        taskEXIT_CRITICAL();

        vTaskDelay(500);
        //vTaskDelay(200);
    }
}

/* USER CODE END */
