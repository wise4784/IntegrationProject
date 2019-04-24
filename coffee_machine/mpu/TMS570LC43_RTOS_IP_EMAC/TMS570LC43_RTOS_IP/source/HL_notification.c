/** @file HL_notification.c 
*   @brief User Notification Definition File
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file  defines  empty  notification  routines to avoid
*   linker errors, Driver expects user to define the notification. 
*   The user needs to either remove this file and use their custom 
*   notification function or place their code sequence in this file 
*   between the provided USER CODE BEGIN and USER CODE END.
*
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* Include Files */

#include "HL_esm.h"
#include "HL_gio.h"
#include "HL_sci.h"
#include "HL_epc.h"
#include "HL_emac.h" 
#include "HL_sys_dma.h"

/* USER CODE BEGIN (0) */
#include "FreeRTOS.h"
#include "os_queue.h"
#include "os_list.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "notification.h"

/* Buffers to retrieve EMAC data at interrupt level */
/* and forward to the IP stack at task level */
static emac_data_t emacRxData[NUMBER_OF_BUFS];
static int emacSetDataIndex = 0;
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
#pragma WEAK(gioNotification)
void gioNotification(gioPORT_t *port, uint32 bit)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (22) */
/* USER CODE END */
}

/* USER CODE BEGIN (23) */
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

#pragma WEAK(epcCAMFullNotification)
void epcCAMFullNotification(void)
{
/*  enter user code between the USER CODE BEGIN and USER CODE END. */
/* USER CODE BEGIN (57) */
/* USER CODE END */
}
#pragma WEAK(epcFIFOFullNotification)
void epcFIFOFullNotification(uint32 epcFIFOStatus)
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

	/*******************************************************/
	/* This method runs at processor interrupt level */
	/*******************************************************/

	uint32_t localBufferSize = 0;

	uint32_t dataCopied = 0;

	/* Used to copy the data from the EMAC to a statically
	 * allocated buffer so a task can put it into a
	 * network buffer and send it to the UDP/IP stack.
	 *
	 */
	uint8_t* pSrcBuf;
	uint8_t* pDestBuf;

    /* I used this to send the data buffer pointer to the task queue */
    long queueData;

	rxch_t *rxch_int;
	volatile emac_rx_bd_t *curr_bd, *curr_tail, *last_bd;

	/* The receive structure that holds data about a particular receive channel */
	rxch_int = &(hdkif->rxchptr);

	/* Get the buffer descriptors which contain the earliest filled data */
	/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
	curr_bd = rxch_int->active_head;
	/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
	last_bd = rxch_int->active_tail;

	/**
	 * Process the descriptors as long as data is available
	 * when the DMA is receiving data, SOP flag will be set
	 */
	/*SAFETYMCUSW 28 D MR:NA <APPROVED> "Hardware status bit read check" */
	/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
	/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
	while((EMACSwizzleData(curr_bd->flags_pktlen) & EMAC_BUF_DESC_SOP) == EMAC_BUF_DESC_SOP) {


		/* Start processing once the packet is loaded */
		/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
		/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
		if((EMACSwizzleData(curr_bd->flags_pktlen) & EMAC_BUF_DESC_OWNER)
				!= EMAC_BUF_DESC_OWNER) {

			/* this bd chain will be freed after processing */
			/*SAFETYMCUSW 71 S MR:17.6 <APPROVED> "Assigned pointer value has required scope." */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			rxch_int->free_head = curr_bd;

			/*
			 * I copied the data to a temporary buffer first because the
			 * stack buffer manager doesn't support getting buffers at ISR
			 */

			/* Initialize the destination data buffer pointer */
			pDestBuf = (uint8_t *)(&emacRxData[emacSetDataIndex].emacData[0]);

			/*
			 * The loop runs till it reaches the end of the packet.
			 */
			/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			while((EMACSwizzleData(curr_bd->flags_pktlen) & EMAC_BUF_DESC_EOP)!= EMAC_BUF_DESC_EOP)
			{

				/* Initialize the source buffer pointer */
				pSrcBuf = (uint8_t *)(EMACSwizzleData(curr_bd->bufptr) + ((EMACSwizzleData(curr_bd->bufoff_len) & 0xFFFF0000) >> 16));
				localBufferSize = (EMACSwizzleData(curr_bd->bufoff_len) & 0x0000FFFF);

				/* Copy the data to the destination buffer */
				memcpy(pDestBuf,          /* Destination */
						pSrcBuf,           /* Source */
						localBufferSize);  /* Number of bytes */

				/* Track the amount of data copied */
				dataCopied += localBufferSize;

				/* Advance the destination pointer */
				pDestBuf += localBufferSize;



				/*Update the flags for the descriptor again and the length of the buffer*/
				/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
				curr_bd->flags_pktlen = EMACSwizzleData((uint32)EMAC_BUF_DESC_OWNER);
				/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
				curr_bd->bufoff_len = EMACSwizzleData((uint32)MAX_TRANSFER_UNIT);
				/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
				last_bd = curr_bd;
				/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
				curr_bd = (emac_rx_bd_t *)EMACSwizzleData((uint32)curr_bd->next);
			}

			/* Process the EOP BD */

			/* Initialize the source buffer pointer */
			pSrcBuf = (uint8_t *)(EMACSwizzleData(curr_bd->bufptr) + ((EMACSwizzleData(curr_bd->bufoff_len) & 0xFFFF0000) >> 16));
			localBufferSize = EMACSwizzleData(curr_bd->bufoff_len) & 0x0000FFFF;

			/* Copy the data to the destination buffer */
			memcpy(pDestBuf,          /* Destination */
					pSrcBuf,           /* Source */
					localBufferSize);  /* Number of bytes */

			/* Track the amount of data copied */
			dataCopied += localBufferSize;

			/* Updating the last descriptor (which contained the EOP flag) */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			curr_bd->flags_pktlen = EMACSwizzleData((uint32)EMAC_BUF_DESC_OWNER);
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			curr_bd->bufoff_len = EMACSwizzleData((uint32)MAX_TRANSFER_UNIT);
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			last_bd = curr_bd;
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			curr_bd = (emac_rx_bd_t *)EMACSwizzleData((uint32)curr_bd->next);

			/* Acknowledge that this packet is processed */
			/*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Address stored in pointer is passed as as an int parameter. - Advisory as per MISRA" */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			EMACRxCPWrite(hdkif->emac_base, (uint32)EMAC_CHANNELNUMBER, (uint32)last_bd);

			/* The next buffer descriptor is the new head of the linked list. */
			/*SAFETYMCUSW 71 S MR:17.6 <APPROVED> "Assigned pointer value has required scope." */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			rxch_int->active_head = curr_bd;

			/* The processed descriptor is now the tail of the linked list. */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			curr_tail = rxch_int->active_tail;
			/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			curr_tail->next = (emac_rx_bd_t *)EMACSwizzleData((uint32)rxch_int->free_head);

			/* The last element in the already processed Rx descriptor chain is now the end of list. */
			/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			last_bd->next = NULL;


			/**
			 * Check if the reception has ended. If the EOQ flag is set, the NULL
			 * Pointer is taken by the DMA engine. So we need to write the RX HDP
			 * with the next descriptor.
			 */
			/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
			/*SAFETYMCUSW 134 S MR:12.2 <APPROVED> "LDRA Tool issue" */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			if((EMACSwizzleData(curr_tail->flags_pktlen) & EMAC_BUF_DESC_EOQ) == EMAC_BUF_DESC_EOQ) {
				/*SAFETYMCUSW 439 S MR:11.3 <APPROVED> "Address stored in pointer is passed as as an int parameter. - Advisory as per MISRA" */
				/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
				EMACRxHdrDescPtrWrite(hdkif->emac_base, (uint32)(rxch_int->free_head), (uint32)EMAC_CHANNELNUMBER);
			}

			/*SAFETYMCUSW 71 S MR:17.6 <APPROVED> "Assigned pointer value has required scope." */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			/*SAFETYMCUSW 45 D MR:21.1 <APPROVED> "Valid non NULL input parameters are assigned in this driver" */
			rxch_int->free_head  = curr_bd;
			rxch_int->active_tail = last_bd;


			/* Force the packet size to the amount of data copied */
			emacRxData[emacSetDataIndex].packetSize = dataCopied;
			dataCopied = 0;

			/*
			 * Notify the receiving task - I tried to do the cast in one step
			 * in the function call, but it didn't work right so I gave up and
			 * did it separately
			 */
			queueData = (long)(&emacRxData[emacSetDataIndex]);
			xQueueSendToBackFromISR(xEmacRxQueue, &queueData, NULL);

			/* Advance the static receive buffer index */
			emacSetDataIndex = (emacSetDataIndex + 1) % NUMBER_OF_BUFS;

		} /* end if((curr_bd->flags_pktlen & EMAC_BUF_DESC_OWNER) != EMAC_BUF_DESC_OWNER) */

	} /* while((curr_bd->flags_pktlen & EMAC_BUF_DESC_SOP) == EMAC_BUF_DESC_SOP) */

	/* USER CODE END */
}

/* USER CODE BEGIN (63) */


/* USER CODE END */
