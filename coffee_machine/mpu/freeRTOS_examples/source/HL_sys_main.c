#include "HL_sys_common.h"
#include "HL_sci.h"
#include "HL_gio.h"
#include <stdio.h>
#include <string.h>
#include "HL_system.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"

#include "os_queue.h"
#include "os_semphr.h"
#include "os_task.h"

xTaskHandle task0;
xTaskHandle task1;
xQueueHandle Global_Queue_Handle=0;
QueueHandle_t mutex=NULL;

char buf[256]={0};
unsigned int buf_len;

void wait (uint32 delay)
{
    int i;
    for(i=0;i<delay;i++)
        ;
}

void sci_display(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((sciREG1->FLR & 0x4) == 4)
            ;
        sciSendByte(sciREG1, *text++);
    }

}

void sender_task(void *p)
{
        int i=0;
        //gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
        while(1){
            if(xSemaphoreTake(mutex, (TickType_t)0x01)==pdTRUE){
                sprintf(buf,"send to queue : %d\n",i);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *) buf, buf_len);

                if(xQueueSend(Global_Queue_Handle, &i, 1000))
                ;
                else{
                    sprintf(buf," Failed to send to queue\n");
                    buf_len = strlen(buf);
                    sci_display(sciREG1, (uint8 *) buf, buf_len);
                }
                ++i;
                gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
            }
            xSemaphoreGive(mutex);
            gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1);
            vTaskDelay(1000);
            //wait(28000000);
        }
}

//xQueueSend( xQueue, pvItemToQueue, xTicksToWait ) xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), ( xTicksToWait ), queueSEND_TO_BACK )

void receiver_task(void *p)
{
    int rx_int=0;
    //gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
    while(1){
        if(xSemaphoreTake(mutex, (TickType_t)0x01)==pdTRUE){
            if(xQueueReceive(Global_Queue_Handle, &rx_int, 1000)) {//wait 1000(1sec) or show fail message
                sprintf(buf," Received %d",rx_int);
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *) buf, buf_len);
            }
            else{
                sprintf(buf," Failed to receive data from queue");
                buf_len = strlen(buf);
                sci_display(sciREG1, (uint8 *) buf, buf_len);
            }
            gioSetBit(gioPORTB, 6, gioGetBit(gioPORTB, 6) ^ 1);
        }
        xSemaphoreGive(mutex);
        vTaskDelay(1000);
 //       wait(28000000);
    }
}

int main(void)
{
    gioInit();
    sciInit();
    gioSetDirection(gioPORTB,0xffffff);
    Global_Queue_Handle= xQueueCreate(3, sizeof(int)); //items hold in queue = 3
    vSemaphoreCreateBinary(mutex);

    gioSetBit(gioPORTB, 7,1);
    gioSetBit(gioPORTB, 6,1);

    sprintf(buf,"queue test start\n\r");
    buf_len = strlen(buf);
    sci_display(sciREG1, (uint8 *) buf, buf_len);

    wait(28000000);
#if 0
    if(xTaskCreate(receiver_task,"rx", configMINIMAL_STACK_SIZE,NULL, 1, NULL)!=pdTRUE)
    {
        for(;;)
            ;
    }
#endif
    if(xTaskCreate(sender_task,"tx", configMINIMAL_STACK_SIZE,NULL,1, NULL)!=pdTRUE)
    {
        for(;;)
            ;
    }
    vTaskStartScheduler();

    return 0;
}
