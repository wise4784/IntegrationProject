/*a}
ssion is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or (b) that interact
* with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in this
* Software without prior written authorization from Xilinx.
*
*/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

//needs to be checked
#define SPI_DEV	"/dev/spidev32766.0"

typedef unsigned char	Xuint8;		/**< unsigned 8-bit */
typedef char		Xint8;		/**< signed 8-bit */
typedef unsigned short	Xuint16;	/**< unsigned 16-bit */
typedef short		Xint16;		/**< signed 16-bit */
typedef unsigned long	Xuint32;	/**< unsigned 32-bit */
typedef long		Xint32;		/**< signed 32-bit */
typedef float		Xfloat32;	/**< 32-bit floating point */
typedef double		Xfloat64;	/**< 64-bit double precision FP */
typedef unsigned long	Xboolean;	/**< boolean (XTRUE or XFALSE) */

typedef char char8;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint64_t u64;
typedef int sint32;

typedef s32 XStatus;

typedef Xuint32         u32;
typedef Xuint16         u16;
typedef Xuint8          u8;

#define TRUE	1
#define FALSE	0

#define XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ 666666687
#define XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ	XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ
#define COUNTS_PER_SECOND          (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ /2)


#define PMODCAN_AXI_LITE_SPI_SIZE		0x10000
#define PMODCAN_AXI_LITE_SPI_BASEADDR	0x40000000
#define PMODCAN_AXI_LITE_GPIO_SIZE		0x1000
#define PMODCAN_AXI_LITE_GPIO_BASEADDR	0x40010000

#define XSP_DGIER_OFFSET	0x1C	/**< Global Intr Enable Reg */
#define XSP_IISR_OFFSET		0x20	/**< Interrupt status Reg */
#define XSP_IIER_OFFSET		0x28	/**< Interrupt Enable Reg */
#define XSP_SRR_OFFSET	 	0x40	/**< Software Reset register */
#define XSP_CR_OFFSET		0x60	/**< Control register */
#define XSP_SR_OFFSET		0x64	/**< Status Register */
#define XSP_DTR_OFFSET		0x68	/**< Data transmit */
#define XSP_DRR_OFFSET		0x6C	/**< Data receive */
#define XSP_SSR_OFFSET		0x70	/**< 32-bit slave select */
#define XSP_TFO_OFFSET		0x74	/**< Tx FIFO occupancy */
#define XSP_RFO_OFFSET		0x78	/**< Rx FIFO occupancy */

#define XSP_MASTER_OPTION		0x1
#define XSP_CLK_ACTIVE_LOW_OPTION	0x2
#define XSP_CLK_PHASE_1_OPTION		0x4
#define XSP_LOOPBACK_OPTION		0x8
#define XSP_MANUAL_SSELECT_OPTION	0x10

#define XSP_DATAWIDTH_BYTE	 8
#define XSP_DATAWIDTH_HALF_WORD	16  /**< Tx/Rx Reg is Half Word (16 bit)
						Wide */
#define XSP_DATAWIDTH_WORD	32  /**< Tx/Rx Reg is Word (32 bit)  Wide */

#define XSP_SRR_RESET_MASK		0x0000000A

#define XSP_GINTR_ENABLE_MASK	0x80000000	/**< Global interrupt enable */

#define XSP_CR_LOOPBACK_MASK	   0x00000001 /**< Local loopback mode */
#define XSP_CR_ENABLE_MASK	   0x00000002 /**< System enable */
#define XSP_CR_MASTER_MODE_MASK	   0x00000004 /**< Enable master mode */
#define XSP_CR_CLK_POLARITY_MASK   0x00000008 /**< Clock polarity high
								or low */
#define XSP_CR_CLK_PHASE_MASK	   0x00000010 /**< Clock phase 0 or 1 */
#define XSP_CR_TXFIFO_RESET_MASK   0x00000020 /**< Reset transmit FIFO */
#define XSP_CR_RXFIFO_RESET_MASK   0x00000040 /**< Reset receive FIFO */
#define XSP_CR_MANUAL_SS_MASK	   0x00000080 /**< Manual slave select
								assert */
#define XSP_CR_TRANS_INHIBIT_MASK  0x00000100 /**< Master transaction
								inhibit */

#define XSP_SR_RX_EMPTY_MASK	   0x00000001 /**< Receive Reg/FIFO is empty */
#define XSP_SR_RX_FULL_MASK	   0x00000002 /**< Receive Reg/FIFO is full */
#define XSP_SR_TX_EMPTY_MASK	   0x00000004 /**< Transmit Reg/FIFO is
								empty */
#define XSP_SR_TX_FULL_MASK	   0x00000008 /**< Transmit Reg/FIFO is full */
#define XSP_SR_MODE_FAULT_MASK	   0x00000010 /**< Mode fault error */
#define XSP_SR_SLAVE_MODE_MASK	   0x00000020 /**< Slave mode select */

#define XIL_COMPONENT_IS_STARTED   0x22222222U
#define XIL_COMPONENT_IS_READY     0x11111111U

#define XST_SUCCESS                     0L
#define XST_FAILURE                     1L
#define XST_DEVICE_NOT_FOUND            2L
#define XST_DEVICE_BLOCK_NOT_FOUND      3L
#define XST_INVALID_VERSION             4L
#define XST_DEVICE_IS_STARTED           5L
#define XST_DEVICE_IS_STOPPED           6L
#define XST_FIFO_ERROR                  7L	/*!< An error occurred during an
						   operation with a FIFO such as
						   an underrun or overrun, this
						   error requires the device to
						   be reset */
#define XST_RESET_ERROR                 8L	/*!< An error occurred which requires
						   the device to be reset */
#define XST_DMA_ERROR                   9L	/*!< A DMA error occurred, this error
						   typically requires the device
						   using the DMA to be reset */
#define XST_NOT_POLLED                  10L	/*!< The device is not configured for
						   polled mode operation */
#define XST_FIFO_NO_ROOM                11L	/*!< A FIFO did not have room to put
						   the specified data into */
#define XST_BUFFER_TOO_SMALL            12L	/*!< The buffer is not large enough
						   to hold the expected data */
#define XST_NO_DATA                     13L	/*!< There was no data available */
#define XST_REGISTER_ERROR              14L	/*!< A register did not contain the
						   expected value */
#define XST_INVALID_PARAM               15L	/*!< An invalid parameter was passed
						   into the function */
#define XST_NOT_SGDMA                   16L	/*!< The device is not configured for
						   scatter-gather DMA operation */
#define XST_LOOPBACK_ERROR              17L	/*!< A loopback test failed */
#define XST_NO_CALLBACK                 18L	/*!< A callback has not yet been
						   registered */
#define XST_NO_FEATURE                  19L	/*!< Device is not configured with
						   the requested feature */
#define XST_NOT_INTERRUPT               20L	/*!< Device is not configured for
						   interrupt mode operation */
#define XST_DEVICE_BUSY                 21L	/*!< Device is busy */
#define XST_ERROR_COUNT_MAX             22L	/*!< The error counters of a device
						   have maxed out */
#define XST_IS_STARTED                  23L	/*!< Used when part of device is
						   already started i.e.
						   sub channel */
#define XST_IS_STOPPED                  24L	/*!< Used when part of device is
						   already stopped i.e.
						   sub channel */
#define XST_DATA_LOST                   26L	/*!< Driver defined error */
#define XST_RECV_ERROR                  27L	/*!< Generic receive error */
#define XST_SEND_ERROR                  28L	/*!< Generic transmit error */
#define XST_NOT_ENABLED                 29L	/*!< A requested service is not
						   available because it has not
						   been enabled */

#define XST_SPI_MODE_FAULT          1151	/*!< master was selected as slave */
#define XST_SPI_TRANSFER_DONE       1152	/*!< data transfer is complete */
#define XST_SPI_TRANSMIT_UNDERRUN   1153	/*!< slave underruns transmit register */
#define XST_SPI_RECEIVE_OVERRUN     1154	/*!< device overruns receive register */
#define XST_SPI_NO_SLAVE            1155	/*!< no slave has been selected yet */
#define XST_SPI_TOO_MANY_SLAVES     1156	/*!< more than one slave is being
						 * selected */
#define XST_SPI_NOT_MASTER          1157	/*!< operation is valid only as master */
#define XST_SPI_SLAVE_ONLY          1158	/*!< device is configured as slave-only
						 */
#define XST_SPI_SLAVE_MODE_FAULT    1159	/*!< slave was selected while disabled */
#define XST_SPI_SLAVE_MODE          1160	/*!< device has been addressed as slave */
#define XST_SPI_RECEIVE_NOT_EMPTY   1161	/*!< device received data in slave mode */

#define XST_SPI_COMMAND_ERROR       1162	/*!< unrecognised command - qspi only */
#define XST_SPI_POLL_DONE           1163        /*!< controller completed polling the
						   device for status */

#define XSP_INTR_MODE_FAULT_MASK	0x00000001 /**< Mode fault error */
#define XSP_INTR_SLAVE_MODE_FAULT_MASK	0x00000002 /**< Selected as slave while
						     *  disabled */
#define XSP_INTR_TX_EMPTY_MASK		0x00000004 /**< DTR/TxFIFO is empty */
#define XSP_INTR_TX_UNDERRUN_MASK	0x00000008 /**< DTR/TxFIFO underrun */
#define XSP_INTR_RX_FULL_MASK		0x00000010 /**< DRR/RxFIFO is full */
#define XSP_INTR_RX_OVERRUN_MASK	0x00000020 /**< DRR/RxFIFO overrun */
#define XSP_INTR_TX_HALF_EMPTY_MASK	0x00000040 /**< TxFIFO is half empty */
#define XSP_INTR_SLAVE_MODE_MASK	0x00000080 /**< Slave select mode */
#define XSP_INTR_RX_NOT_EMPTY_MASK	0x00000100 /**< RxFIFO not empty */

#define XSP_INTR_CPOL_CPHA_ERR_MASK	0x00000200 /**< CPOL/CPHA error */
#define XSP_INTR_SLAVE_MODE_ERR_MASK	0x00000400 /**< Slave mode error */
#define XSP_INTR_MSB_ERR_MASK		0x00000800 /**< MSB Error */
#define XSP_INTR_LOOP_BACK_ERR_MASK	0x00001000 /**< Loop back error */
#define XSP_INTR_CMD_ERR_MASK		0x00002000 /**< 'Invalid cmd' error */

#define XSP_INTR_DFT_MASK	(XSP_INTR_MODE_FAULT_MASK |	\
				 XSP_INTR_TX_UNDERRUN_MASK |	\
				 XSP_INTR_RX_OVERRUN_MASK |	\
				 XSP_INTR_SLAVE_MODE_FAULT_MASK | \
				 XSP_INTR_CMD_ERR_MASK)

#define XSP_INTR_ALL		(XSP_INTR_MODE_FAULT_MASK | \
				 XSP_INTR_SLAVE_MODE_FAULT_MASK | \
				 XSP_INTR_TX_EMPTY_MASK | \
				 XSP_INTR_TX_UNDERRUN_MASK | \
				 XSP_INTR_RX_FULL_MASK | \
				 XSP_INTR_TX_HALF_EMPTY_MASK | \
				 XSP_INTR_RX_OVERRUN_MASK | \
				 XSP_INTR_SLAVE_MODE_MASK | \
				 XSP_INTR_RX_NOT_EMPTY_MASK | \
				 XSP_INTR_CMD_ERR_MASK | \
				 XSP_INTR_LOOP_BACK_ERR_MASK | \
				 XSP_INTR_MSB_ERR_MASK | \
				 XSP_INTR_SLAVE_MODE_ERR_MASK | \
				 XSP_INTR_CPOL_CPHA_ERR_MASK)

#define CAN_MODIFY_REG_CMD        0x05

#define CAN_WRITE_REG_CMD         0x02

#define CAN_READ_REG_CMD          0x03

#define CAN_RTS_CMD               0x80
#define CAN_RTS_TXB0_MASK         0x01
#define CAN_RTS_TXB1_MASK         0x02
#define CAN_RTS_TXB2_MASK         0x04

#define CAN_LOADBUF_CMD           0x40
#define CAN_LOADBUF_TXB0SIDH      0x00
#define CAN_LOADBUF_TXB0D0        0x01
#define CAN_LOADBUF_TXB1SIDH      0x02
#define CAN_LOADBUF_TXB1D0        0x03
#define CAN_LOADBUF_TXB2SIDH      0x04
#define CAN_LOADBUF_TXB2D0        0x05

#define CAN_READBUF_CMD           0x90
#define CAN_READBUF_RXB0SIDH      0x00
#define CAN_READBUF_RXB0D0        0x01
#define CAN_READBUF_RXB1SIDH      0x02
#define CAN_READBUF_RXB1D0        0x03

#define CAN_READSTATUS_CMD        0xA0
#define CAN_STATUS_RX0IF_MASK     0x01
#define CAN_STATUS_RX1IF_MASK     0x02
#define CAN_STATUS_TX0REQ_MASK    0x04
#define CAN_STATUS_TX0IF_MASK     0x08
#define CAN_STATUS_TX1REQ_MASK    0x10
#define CAN_STATUS_TX1IF_MASK     0x20
#define CAN_STATUS_TX2REQ_MASK    0x40
#define CAN_STATUS_TX2IF_MASK     0x80

#define CAN_RXSTATUS_CMD          0xB0
#define CAN_RXSTATUS_RX0IF_MASK   0x40
#define CAN_RXSTATUS_RX1IF_MASK   0x80

#define CAN_CANCTRL_REG_ADDR      0x0F
#define CAN_CNF3_REG_ADDR         0x28
#define CAN_CNF2_REG_ADDR         0x29
#define CAN_CNF1_REG_ADDR         0x2A
#define CAN_RXB0CTRL_REG_ADDR     0x60

#define CAN_TXB0CTRL_REG_ADDR     0x30
#define CAN_TXB0CTRL_TXREQ_MASK   0x08

#define CAN_CANINTF_REG_ADDR      0x2C
#define CAN_CANINTF_RX0IF_MASK    0x01
#define CAN_CANINTF_RX1IF_MASK    0x02
#define CAN_CANINTF_TX0IF_MASK    0x04
#define CAN_CANINTF_TX1IF_MASK    0x08
#define CAN_CANINTF_TX2IF_MASK    0x10
#define CAN_CAN_CANCTRL_MODE_MASK 0xE0
#define CAN_CANCTRL_MODE_BIT      5

typedef void (*XSpi_StatusHandler) (void *CallBackRef, u32 StatusEvent,	unsigned int ByteCount);

typedef enum CAN_TxBuffer {
   CAN_Tx0 = 0, CAN_Tx1, CAN_Tx2
} CAN_TxBuffer;

typedef enum CAN_Mode {
   CAN_ModeNormalOperation = 0,
   CAN_ModeSleep,
   CAN_ModeLoopback,
   CAN_ModeListenOnly,
   CAN_ModeConfiguration
} CAN_Mode;

typedef struct CAN_Message {
   u16 id;     // 11 bit id
   u32 eid;    // 18 bit extended id
   u8 ide;     // 1 to enable sending extended id
   u8 rtr;     // Remote transmission request bit
   u8 srr;     // Standard Frame Remote Transmit Request
   u8 dlc;     // Data length
   u8 data[8]; // Data buffer
   // Some additional information has not yet been encapsulated here
   // (ex:priority bits), primarily, no TXBxCTRL bits
} CAN_Message;

typedef struct {
	u32 ModeFaults;		/**< Number of mode fault errors */
	u32 XmitUnderruns;	/**< Number of transmit underruns */
	u32 RecvOverruns;	/**< Number of receive overruns */
	u32 SlaveModeFaults;	/**< Num of selects as slave while disabled */
	u32 BytesTransferred;	/**< Number of bytes transferred */
	u32 NumInterrupts;	/**< Number of transmit/receive interrupts */
} XSpi_Stats;

typedef struct {
	XSpi_Stats Stats;	/**< Statistics */
	u32 BaseAddr;		/**< Base address of device (IPIF) */
	int IsReady;		/**< Device is initialized and ready */
	int IsStarted;		/**< Device has been started */
	int HasFifos;		/**< Device is configured with FIFOs or not */
	u32 SlaveOnly;		/**< Device is configured to be slave only */
	u8 NumSlaveBits;	/**< Number of slave selects for this device */
	u8 DataWidth;		/**< Data Transfer Width 8 or 16 or 32 */
	u8 SpiMode;		/**< Standard/Dual/Quad mode */
	u32 SlaveSelectMask;	/**< Mask that matches the number of SS bits */
	u32 SlaveSelectReg;	/**< Slave select register */

	u8 *SendBufferPtr;	/**< Buffer to send  */
	u8 *RecvBufferPtr;	/**< Buffer to receive */
	unsigned int RequestedBytes; /**< Total bytes to transfer (state) */
	unsigned int RemainingBytes; /**< Bytes left to transfer (state) */
	int IsBusy;		/**< A transfer is in progress (state) */

	XSpi_StatusHandler StatusHandler; /**< Status Handler */
	void *StatusRef;	/**< Callback reference for status handler */
	u32 FlashBaseAddr;    	/**< Used in XIP Mode */
	u8 XipMode;             /**< 0 if Non-XIP, 1 if XIP Mode */
} XSpi;

typedef struct {
	u16 DeviceId;		/**< Unique ID  of device */
	u32 BaseAddress;	/**< Base address of the device */
	int HasFifos;		/**< Does device have FIFOs? */
	u32 SlaveOnly;		/**< Is the device slave only? */
	u8 NumSlaveBits;	/**< Num of slave select bits on the device */
	u8 DataWidth;		/**< Data transfer Width */
	u8 SpiMode;		/**< Standard/Dual/Quad mode */
	u8 AxiInterface;	/**< AXI-Lite/AXI Full Interface */
	u32 AxiFullBaseAddress;	/**< AXI Full Interface Base address of
					the device */
	u8 XipMode;             /**< 0 if Non-XIP, 1 if XIP Mode */
	u8 Use_Startup;		/**< 1 if Starup block is used in h/w */
} XSpi_Config;

typedef struct PmodCAN {
   u32 GPIO_addr;
   XSpi CANSpi;
} PmodCAN;

XSpi_Config CANConfig =
{
   0,
   0,
   1,
   0,
   1,
   8,
   0,
   0,
   0,
   0,
   0
};

typedef struct {
	u32 Option;
	u32 Mask;
} OptionsMap;

static OptionsMap OptionsTable[] = {
	{XSP_LOOPBACK_OPTION, XSP_CR_LOOPBACK_MASK},
	{XSP_CLK_ACTIVE_LOW_OPTION, XSP_CR_CLK_POLARITY_MASK},
	{XSP_CLK_PHASE_1_OPTION, XSP_CR_CLK_PHASE_MASK},
	{XSP_MASTER_OPTION, XSP_CR_MASTER_MODE_MASK},
	{XSP_MANUAL_SSELECT_OPTION, XSP_CR_MANUAL_SS_MASK}
};

void Initialize();
void Run();
void Cleanup();
void PrintMessage(CAN_Message message);
CAN_Message ComposeMessage();

void CAN_begin(PmodCAN * InstancePtr, u32 GPIO_ADDR, u32 SPI_ADDR);
void CAN_Configure(PmodCAN * InstancePtr, u8 mod);

int CAN_SPIInit(XSpi * SpiInstancePtr);
void CAN_Configure(PmodCAN *InstancePtr, u8 mode);
void CAN_ModifyReg(PmodCAN *InstancePtr, u8 reg, u8 mask, u8 value);
void CAN_WriteReg(PmodCAN *InstancePtr, u8 reg, u8 *data, u32 nData);
void CAN_ClearReg(PmodCAN *InstancePtr, u8 reg, u32 nData);

u8 CAN_ReadStatus(PmodCAN *InstancePtr);
XStatus CAN_SendMessage(PmodCAN *InstancePtr, CAN_Message message, CAN_TxBuffer target);
void CAN_LoadTxBuffer(PmodCAN *InstancePtr, u8 start_addr, u8 *data, u32 nData);
void CAN_RequestToSend(PmodCAN *InstancePtr, u8 mask);
void CAN_end(PmodCAN *InstancePtr);

int XSpi_CfgInitialize(XSpi * InstancePtr, XSpi_Config * Config, u32 EffectiveAddr);
int XSpi_SetOptions(XSpi *InstancePtr, u32 Options);
int XSpi_SetSlaveSelect(XSpi *InstancePtr, u32 SlaveMask);
int XSpi_Start(XSpi *InstancePtr);
void XSpi_Reset(XSpi *InstancePtr);
void XSpi_Abort(XSpi *InstancePtr);
int XSpi_Transfer(XSpi *InstancePtr, u8 *SendBufPtr, u8 *RecvBufPtr, unsigned int ByteCount);
int XSpi_Stop(XSpi *InstancePtr);

static void do_msg(int fd, int len);
static void do_read(int fd, int len);
static void dumpstat(const char *name, int fd);

static inline void Xil_Out32(u32 Addr, u32 Value)
{
	printf("Xil_Out32\n");
	volatile u32 *LocalAddr = (volatile u32 *)Addr;
	*LocalAddr = Value;
}

static inline u32 Xil_In32(u32 Addr)
{
	printf("Xil_In32\n");
	return *(volatile u32 *) Addr;
}

#define XSpi_Out32	Xil_Out32
#define XSpi_In32	Xil_In32

#define XSpi_WriteReg(BaseAddress, RegOffset, RegisterValue) \
	XSpi_Out32(*((u32 *)ptr) + (RegOffset), (RegisterValue))

#define XSpi_ReadReg(BaseAddress, RegOffset) \
	XSpi_In32(*((u32*)ptr) + (RegOffset))

#define XSpi_GetStatusReg(InstancePtr) \
	XSpi_ReadReg(((InstancePtr)->BaseAddr), XSP_SR_OFFSET)

#define XSpi_GetControlReg(InstancePtr) \
	XSpi_ReadReg(((InstancePtr)->BaseAddr), XSP_CR_OFFSET)

#define XSpi_SetControlReg(InstancePtr, Mask) \
	XSpi_WriteReg(((InstancePtr)->BaseAddr), XSP_CR_OFFSET, (Mask))

#define XSpi_SetSlaveSelectReg(InstancePtr, Mask) \
	XSpi_WriteReg(((InstancePtr)->BaseAddr), XSP_SSR_OFFSET, (Mask))

#define XSpi_IntrClear(InstancePtr, ClearMask) 			\
	XSpi_WriteReg(((InstancePtr)->BaseAddr),  XSP_IISR_OFFSET,	\
		XSpi_IntrGetStatus(InstancePtr) | (ClearMask))

#define XSpi_IntrGetStatus(InstancePtr) \
	XSpi_ReadReg(((InstancePtr)->BaseAddr), XSP_IISR_OFFSET)

#define XSpi_IntrGlobalEnable(InstancePtr)				\
	XSpi_WriteReg(((InstancePtr)->BaseAddr),  XSP_DGIER_OFFSET, 	\
			XSP_GINTR_ENABLE_MASK)

#define XSpi_IntrEnable(InstancePtr, EnableMask)			\
	XSpi_WriteReg(((InstancePtr)->BaseAddr), XSP_IIER_OFFSET,	\
		(XSpi_ReadReg(((InstancePtr)->BaseAddr), 		\
			XSP_IIER_OFFSET)) | (((EnableMask) & XSP_INTR_ALL )))

#define XSpi_IntrGlobalDisable(InstancePtr) 				\
	XSpi_WriteReg(((InstancePtr)->BaseAddr),  XSP_DGIER_OFFSET, 0)

#define XSpi_IsIntrGlobalEnabled(InstancePtr)				\
	(XSpi_ReadReg(((InstancePtr)->BaseAddr), XSP_DGIER_OFFSET) ==  \
		XSP_GINTR_ENABLE_MASK)

#define XSP_NUM_OPTIONS		(sizeof(OptionsTable) / sizeof(OptionsMap))

PmodCAN myDevice;
void * ptr;

int main(void)
{
	int fd, i;
	int rbuf[128];
	const char * name = SPI_DEV;
	int readcount = 8;
	int msglen = 8;

	char wr_buf[] = {0xff, 0x00, 0x1f, 0x0f};
	char rd_buf[10];

	if((fd = open(SPI_DEV, O_RDWR))<0)
	{
		perror("Open error: ");
		exit(-1);
	}
	printf("Open success\n");

	dumpstat(name, fd);

	memset(rbuf, 0, sizeof(rbuf));

	read(fd, rbuf, 128);

	for(i=0; i<128; i++)
		printf("rbuf[%d] = %d\n", i, rbuf[i]);


/*
	if(write(fd, wr_buf, sizeof(wr_buf))!=sizeof(wr_buf))
	{
		perror("write error");
	}

	if(read(fd, rd_buf, sizeof(rd_buf))!=sizeof(rd_buf))
	{
		perror("read error");
	}


	printf("before do_msg\n");

	if (msglen)
		do_msg(fd, msglen);

	printf("after do_msg\n");

	if (readcount)
		do_read(fd, readcount);


	ptr = mmap((void *)PMODCAN_AXI_LITE_SPI_BASEADDR, PMODCAN_AXI_LITE_SPI_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED)
	{
		perror("mmap error: ");
		exit(-1);
	}
	printf("mmap success\n");

	
	read(fd, rbuf, sizeof(rbuf));
	for(i=0; i<128; i++)
	{
		printf("rbuf[%d] = %d\n", i, rbuf[i]);
	}
	
	Initialize();
	Run();
	Cleanup();	

	close(fd);
*/
    return 0;
}

static void do_read(int fd, int len)
{
	unsigned char	buf[32], *bp;
	int		status;

	/* read at least 2 bytes, no more than 32 */
	if (len < 2)
		len = 2;
	else if (len > sizeof(buf))
		len = sizeof(buf);
	memset(buf, 0, sizeof buf);

	status = read(fd, buf, len);
	if (status < 0) {
		perror("read");
		return;
	}
	if (status != len) {
		fprintf(stderr, "short read\n");
		return;
	}

	printf("read(%2d, %2d): %02x %02x,", len, status,
		buf[0], buf[1]);
	status -= 2;
	bp = buf + 2;
	while (status-- > 0)
		printf(" %02x", *bp++);
	printf("\n");
}

static void do_msg(int fd, int len)
{
	struct spi_ioc_transfer	xfer[2];
	unsigned char		buf[32], *bp;
	int			status;

	memset(xfer, 0, sizeof xfer);
	memset(buf, 0, sizeof buf);

	if (len > sizeof buf)
		len = sizeof buf;

	buf[0] = 0xaa;
	xfer[0].tx_buf = (unsigned long)buf;
	xfer[0].len = 1;

	xfer[1].rx_buf = (unsigned long) buf;
	xfer[1].len = len;

	printf("before do_msg ioctl\n");

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);

	printf("after do_msg ioctl\n");
	if (status < 0) {

		printf("status : %d\n", status);
		perror("SPI_IOC_MESSAGE");
		return;
	}

	printf("response(%2d, %2d): ", len, status);
	for (bp = buf; len; len--)
		printf(" %02x", *bp++);
	printf("\n");
}

static void dumpstat(const char *name, int fd)
{
	__u8	lsb, bits;
	__u32	mode, speed;

	if (ioctl(fd, SPI_IOC_RD_MODE32, &mode) < 0) {
		perror("SPI rd_mode");
		return;
	}
	if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
		perror("SPI rd_lsb_fist");
		return;
	}
	if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
		perror("SPI bits_per_word");
		return;
	}
	if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
		perror("SPI max_speed_hz");
		return;
	}
	
	printf("%s: spi read mode 0x%x, %d bits %sper word, %d Hz max\n",
		name, mode, bits, lsb ? "(lsb first) " : "", speed);

}

void Initialize()
{
	printf("Initialize\n");
	CAN_begin(&myDevice, PMODCAN_AXI_LITE_GPIO_BASEADDR, PMODCAN_AXI_LITE_SPI_BASEADDR);
	CAN_Configure(&myDevice, CAN_ModeNormalOperation);
}

void CAN_begin(PmodCAN * InstancePtr, u32 GPIO_ADDR, u32 SPI_ADDR)
{
	printf("CAN_begin\n");
	InstancePtr->GPIO_addr = GPIO_ADDR;
	CANConfig.BaseAddress = SPI_ADDR;

	CAN_SPIInit(&InstancePtr->CANSpi);
	
}

void CAN_Configure(PmodCAN *InstancePtr, u8 mode)
{
	printf("CAN_Configure\n");

	u8 CNF[3] = {0x86, 0xFB, 0x41};

   // Set CAN control mode to configuration
   CAN_ModifyReg(InstancePtr, CAN_CANCTRL_REG_ADDR, CAN_CAN_CANCTRL_MODE_MASK,
         CAN_ModeConfiguration << CAN_CANCTRL_MODE_BIT);

   // Set config rate and clock for CAN
   CAN_WriteReg(InstancePtr, CAN_CNF3_REG_ADDR, CNF, 3);

   CAN_ClearReg(InstancePtr, 0x00, 12); // Initiate CAN buffer filters and
   CAN_ClearReg(InstancePtr, 0x10, 12); // registers
   CAN_ClearReg(InstancePtr, 0x20, 8);
   CAN_ClearReg(InstancePtr, 0x30, 14);
   CAN_ClearReg(InstancePtr, 0x40, 14);
   CAN_ClearReg(InstancePtr, 0x50, 14);

   // Set the CAN mode for any message type
   CAN_ModifyReg(InstancePtr, CAN_RXB0CTRL_REG_ADDR, 0x64, 0x60);

   // Set CAN control mode to selected mode (exit configuration)
   CAN_ModifyReg(InstancePtr, CAN_CANCTRL_REG_ADDR, CAN_CAN_CANCTRL_MODE_MASK,
         mode << CAN_CANCTRL_MODE_BIT);

}

int CAN_SPIInit(XSpi * SpiInstancePtr)
{
	printf("CAN_SPIInit\n");
	int Status;

	Status = XSpi_CfgInitialize(SpiInstancePtr, &CANConfig, CANConfig.BaseAddress);

	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	u32 options = (XSP_MASTER_OPTION | XSP_CLK_ACTIVE_LOW_OPTION
          | XSP_MANUAL_SSELECT_OPTION);

	Status = XSpi_SetOptions(SpiInstancePtr, options);
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	Status = XSpi_SetSlaveSelect(SpiInstancePtr, 1);
	if(Status != XST_SUCCESS){
		return XST_FAILURE;
	}

	XSpi_Start(SpiInstancePtr);

	XSpi_IntrGlobalDisable(SpiInstancePtr);

	return XST_SUCCESS;

}

void CAN_ModifyReg(PmodCAN *InstancePtr, u8 reg, u8 mask, u8 value)
{
	printf("CAN_ModifyReg\n");
   u8 buf[4] = {CAN_MODIFY_REG_CMD, reg, mask, value};
   XSpi_Transfer(&InstancePtr->CANSpi, buf, NULL, 4);
}

void CAN_WriteReg(PmodCAN *InstancePtr, u8 reg, u8 *data, u32 nData) {
	printf("CAN_WriteReg\n");
   u8 buf[nData + 2];
   u32 i;
   buf[0] = CAN_WRITE_REG_CMD;
   buf[1] = reg;
   for (i = 0; i < nData; i++)
      buf[i + 2] = data[i];
   XSpi_Transfer(&InstancePtr->CANSpi, buf, NULL, nData + 2);
}

void CAN_ClearReg(PmodCAN *InstancePtr, u8 reg, u32 nData) {
	printf("CAN_ClearReg\n");
   u8 buf[nData + 2];
   buf[0] = CAN_WRITE_REG_CMD;
   buf[1] = reg;
   XSpi_Transfer(&InstancePtr->CANSpi, buf, NULL, nData + 2);
}

u8 CAN_ReadStatus(PmodCAN *InstancePtr) {
	printf("CAN_ReadStatus\n");
   u8 buf[2] = {CAN_READSTATUS_CMD, 0x00};
   XSpi_Transfer(&InstancePtr->CANSpi, buf, buf, 2);
   return buf[1];
}

XStatus CAN_SendMessage(PmodCAN *InstancePtr, CAN_Message message,
      CAN_TxBuffer target) {
	printf("CAN_SendMessage\n");
   u8 data[13];
   u8 i;

   u8 rts_mask;
   u8 load_start_addr;

   switch (target) {
   case CAN_Tx0:
      rts_mask = CAN_RTS_TXB0_MASK;
      load_start_addr = CAN_LOADBUF_TXB0SIDH;
      break;
   case CAN_Tx1:
      rts_mask = CAN_RTS_TXB1_MASK;
      load_start_addr = CAN_LOADBUF_TXB1SIDH;
      break;
   case CAN_Tx2:
      rts_mask = CAN_RTS_TXB2_MASK;
      load_start_addr = CAN_LOADBUF_TXB2SIDH;
      break;
   default:
      return XST_FAILURE;
   }

   data[0] = (message.id >> 3) & 0xFF; // TXB0 SIDH

   data[1] = (message.id << 5) & 0xE0; // TXB0 SIDL
   data[1] |= (message.ide << 3) & 0x08;
   data[1] |= (message.eid >> 16) & 0x03;

   data[2] = (message.eid >> 8) & 0xFF;
   data[3] = (message.eid) & 0xFF;

   data[4] = (message.rtr << 6) & 0x40;
   data[4] |= (message.dlc) & 0x0F;

   for (i = 0; i < message.dlc; i++)
      data[i + 5] = message.data[i];

//   xil_printf("CAN_SendMessage message.dlc: %02x\r\n", message.dlc);
	printf("CAN_SendMessage message.dlc: %02x\r\n", message.dlc);
   for (i = 0; i < 5 + message.dlc; i++)
		printf("CAN_SendMessage: %02x\r\n", data[i]);
//      xil_printf("CAN_SendMessage: %02x\r\n", data[i]);

   CAN_LoadTxBuffer(InstancePtr, load_start_addr, data, message.dlc + 5);
   CAN_RequestToSend(InstancePtr, rts_mask);

   return XST_SUCCESS;
}

void CAN_LoadTxBuffer(PmodCAN *InstancePtr, u8 start_addr, u8 *data,
      u32 nData) {
	printf("CAN_LoadTxBuffet\n");
   u8 buf[nData + 1];
   u32 i;
   buf[0] = CAN_LOADBUF_CMD | start_addr;
   for (i = 0; i < nData; i++)
      buf[i + 1] = data[i];
   XSpi_Transfer(&InstancePtr->CANSpi, buf, NULL, nData + 1);
}

void CAN_RequestToSend(PmodCAN *InstancePtr, u8 mask) {
	printf("CAN_RequestToSend\n");
   u8 buf[1] = {CAN_RTS_CMD | mask};
   XSpi_Transfer(&InstancePtr->CANSpi, buf, NULL, 1);
}

void CAN_end(PmodCAN *InstancePtr) {
	printf("CAN_end\n");
   XSpi_Stop(&InstancePtr->CANSpi);
}

int XSpi_CfgInitialize(XSpi * InstancePtr, XSpi_Config * Config, u32 EffectiveAddr)
{
	printf("XSpi_CfgInitialize\n");
	u8 Buffer[3];
	u32 ControlReg;

	if (InstancePtr->IsStarted == XIL_COMPONENT_IS_STARTED) {
		return XST_DEVICE_IS_STARTED;
	}

	InstancePtr->IsStarted = 0;
	InstancePtr->IsBusy = FALSE;

//	InstancePtr->StatusHandler = StubStatusHandler;

	InstancePtr->SendBufferPtr = NULL;
	InstancePtr->RecvBufferPtr = NULL;
	InstancePtr->RequestedBytes = 0;
	InstancePtr->RemainingBytes = 0;
	InstancePtr->BaseAddr = EffectiveAddr;
	InstancePtr->HasFifos = Config->HasFifos;
	InstancePtr->SlaveOnly = Config->SlaveOnly;
	InstancePtr->NumSlaveBits = Config->NumSlaveBits;
	if (Config->DataWidth == 0) {
		InstancePtr->DataWidth = XSP_DATAWIDTH_BYTE;
	} else {
		InstancePtr->DataWidth = Config->DataWidth;
	}

	InstancePtr->SpiMode = Config->SpiMode;

	InstancePtr->FlashBaseAddr = Config->AxiFullBaseAddress;
	InstancePtr->XipMode = Config->XipMode;

	InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

	InstancePtr->SlaveSelectMask = (1 << InstancePtr->NumSlaveBits) - 1;
	InstancePtr->SlaveSelectReg = InstancePtr->SlaveSelectMask;

	InstancePtr->Stats.ModeFaults = 0;
	InstancePtr->Stats.XmitUnderruns = 0;
	InstancePtr->Stats.RecvOverruns = 0;
	InstancePtr->Stats.SlaveModeFaults = 0;
	InstancePtr->Stats.BytesTransferred = 0;
	InstancePtr->Stats.NumInterrupts = 0;

	if(Config->Use_Startup == 1) {
	
		ControlReg = XSpi_GetControlReg(InstancePtr);
		ControlReg |= XSP_CR_TXFIFO_RESET_MASK | XSP_CR_RXFIFO_RESET_MASK |
				XSP_CR_ENABLE_MASK | XSP_CR_MASTER_MODE_MASK ;
		XSpi_SetControlReg(InstancePtr, ControlReg);

		Buffer[0] = 0x9F;
		Buffer[1] = 0x00;
		Buffer[2] = 0x00;

		XSpi_WriteReg(InstancePtr->BaseAddr, XSP_DTR_OFFSET, Buffer[0]);
		XSpi_WriteReg(InstancePtr->BaseAddr, XSP_DTR_OFFSET, Buffer[1]);
		XSpi_WriteReg(InstancePtr->BaseAddr, XSP_DTR_OFFSET, Buffer[2]);

		ControlReg = XSpi_GetControlReg(InstancePtr);
		ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
		XSpi_SetControlReg(InstancePtr, ControlReg);

		ControlReg = XSpi_GetControlReg(InstancePtr);
		ControlReg |= XSP_CR_TRANS_INHIBIT_MASK;
		XSpi_SetControlReg(InstancePtr, ControlReg);

		XSpi_ReadReg(InstancePtr->BaseAddr, XSP_DRR_OFFSET);
		XSpi_ReadReg(InstancePtr->BaseAddr, XSP_DRR_OFFSET);		

	}

	XSpi_Reset(InstancePtr);	

	return XST_SUCCESS;
}

int XSpi_SetOptions(XSpi *InstancePtr, u32 Options)
{
	printf("XSpi_SetOptions\n");

	u32 ControlReg;
	u32 Index;

	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}
	if ((Options & XSP_MASTER_OPTION) && (InstancePtr->SlaveOnly)) {
		return XST_SPI_SLAVE_ONLY;
	}

	ControlReg = XSpi_GetControlReg(InstancePtr);

	for (Index = 0; Index < XSP_NUM_OPTIONS; Index++) {
		if (Options & OptionsTable[Index].Option) {
			ControlReg |= OptionsTable[Index].Mask;
		}
		else {
			ControlReg &= ~OptionsTable[Index].Mask;
		}
	}

	XSpi_SetControlReg(InstancePtr, ControlReg);

	return XST_SUCCESS;
}

int XSpi_SetSlaveSelect(XSpi *InstancePtr, u32 SlaveMask)
{
	printf("XSpi_SetSlaveSelect\n");

	int NumAsserted;
	int Index;

	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}

	NumAsserted = 0;
	for (Index = (InstancePtr->NumSlaveBits - 1); Index >= 0; Index--) {
		if ((SlaveMask >> Index) & 0x1) {
			NumAsserted++;
		}
	}

	if (NumAsserted > 1) {
		return XST_SPI_TOO_MANY_SLAVES;
	}

	InstancePtr->SlaveSelectReg = ~SlaveMask;

	return XST_SUCCESS;
}	

int XSpi_Start(XSpi *InstancePtr)
{
	printf("XSpi_Start\n");

	u32 ControlReg;

	if (InstancePtr->IsStarted == XIL_COMPONENT_IS_STARTED) {
		return XST_DEVICE_IS_STARTED;
	}

	XSpi_IntrEnable(InstancePtr, XSP_INTR_DFT_MASK);

	InstancePtr->IsStarted = XIL_COMPONENT_IS_STARTED;

	ControlReg = XSpi_GetControlReg(InstancePtr);
	ControlReg |= XSP_CR_TXFIFO_RESET_MASK | XSP_CR_RXFIFO_RESET_MASK |
			XSP_CR_ENABLE_MASK;
	XSpi_SetControlReg(InstancePtr, ControlReg);

	XSpi_IntrGlobalEnable(InstancePtr);

	return XST_SUCCESS;
}

void XSpi_Reset(XSpi * InstancePtr)
{
	printf("XSpi_Reset\n");

	XSpi_Abort(InstancePtr);

	InstancePtr->IsStarted = 0;
	InstancePtr->SlaveSelectReg = InstancePtr->SlaveSelectMask;

	XSpi_WriteReg(InstancePtr->BaseAddr, XSP_SRR_OFFSET,
			XSP_SRR_RESET_MASK);
}

void XSpi_Abort(XSpi *InstancePtr){

	printf("XSpi_Abort\n");

	u16 ControlReg;
	
	printf("XSpi_SetSlaveSelectReg\n");
	printf("SlaveSelectMask %d\n", InstancePtr->SlaveSelectMask);
	XSpi_SetSlaveSelectReg(InstancePtr,	InstancePtr->SlaveSelectMask);
	
	printf("Xspi_GetControlReg\n");
	ControlReg = XSpi_GetControlReg(InstancePtr);

	ControlReg |= XSP_CR_TRANS_INHIBIT_MASK;

	if (InstancePtr->HasFifos) {
		ControlReg |= (XSP_CR_TXFIFO_RESET_MASK |
				XSP_CR_RXFIFO_RESET_MASK);
	}

	printf("XSpi_SetControlReg\n");
	XSpi_SetControlReg(InstancePtr, ControlReg);

	InstancePtr->RemainingBytes = 0;
	InstancePtr->RequestedBytes = 0;
	InstancePtr->IsBusy = FALSE;

}

int XSpi_Transfer(XSpi *InstancePtr, u8 *SendBufPtr, u8 *RecvBufPtr, unsigned int ByteCount)
{
	printf("XSpi_Transfer\n");

	u32 ControlReg;
	u32 GlobalIntrReg;
	u32 StatusReg;
	u32 Data = 0;
	u8  DataWidth;

	if (InstancePtr->IsStarted != XIL_COMPONENT_IS_STARTED) {
		return XST_DEVICE_IS_STOPPED;
	}

	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}

	GlobalIntrReg = XSpi_IsIntrGlobalEnabled(InstancePtr);

	XSpi_IntrGlobalDisable(InstancePtr);

	ControlReg = XSpi_GetControlReg(InstancePtr);

	if (ControlReg & XSP_CR_MASTER_MODE_MASK) {
		if ((ControlReg & XSP_CR_LOOPBACK_MASK) == 0) {
			if (InstancePtr->SlaveSelectReg ==
				InstancePtr->SlaveSelectMask) {
				if (GlobalIntrReg == TRUE) {
					XSpi_IntrGlobalEnable(InstancePtr);
				}
				return XST_SPI_NO_SLAVE;
			}
		}
	}

	InstancePtr->IsBusy = TRUE;

	InstancePtr->SendBufferPtr = SendBufPtr;
	InstancePtr->RecvBufferPtr = RecvBufPtr;

	InstancePtr->RequestedBytes = ByteCount;
	InstancePtr->RemainingBytes = ByteCount;

	DataWidth = InstancePtr->DataWidth;

	StatusReg = XSpi_GetStatusReg(InstancePtr);

	while (((StatusReg & XSP_SR_TX_FULL_MASK) == 0) &&
		(InstancePtr->RemainingBytes > 0)) {
		if (DataWidth == XSP_DATAWIDTH_BYTE) {
			Data = *InstancePtr->SendBufferPtr;
		} else if (DataWidth == XSP_DATAWIDTH_HALF_WORD) {
			Data = *(u16 *)InstancePtr->SendBufferPtr;
		} else if (DataWidth == XSP_DATAWIDTH_WORD){
			Data = *(u32 *)InstancePtr->SendBufferPtr;
		}

		XSpi_WriteReg(InstancePtr->BaseAddr, XSP_DTR_OFFSET, Data);
		InstancePtr->SendBufferPtr += (DataWidth >> 3);
		InstancePtr->RemainingBytes -= (DataWidth >> 3);
		StatusReg = XSpi_GetStatusReg(InstancePtr);
	}

	XSpi_SetSlaveSelectReg(InstancePtr,	InstancePtr->SlaveSelectReg);
				
	ControlReg = XSpi_GetControlReg(InstancePtr);
	ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
	XSpi_SetControlReg(InstancePtr, ControlReg);

	if (GlobalIntrReg == TRUE) { /* Interrupt Mode of operation */

		XSpi_IntrEnable(InstancePtr, XSP_INTR_TX_EMPTY_MASK);

		XSpi_IntrGlobalEnable(InstancePtr);

	} else { /* Polled mode of operation */

		while(ByteCount > 0) {

			do {
				StatusReg = XSpi_IntrGetStatus(InstancePtr);
			} while ((StatusReg & XSP_INTR_TX_EMPTY_MASK) == 0);

			XSpi_IntrClear(InstancePtr,XSP_INTR_TX_EMPTY_MASK);

			ControlReg = XSpi_GetControlReg(InstancePtr);
			XSpi_SetControlReg(InstancePtr, ControlReg |
						XSP_CR_TRANS_INHIBIT_MASK);

			StatusReg = XSpi_GetStatusReg(InstancePtr);

			while ((StatusReg & XSP_SR_RX_EMPTY_MASK) == 0) {

				Data = XSpi_ReadReg(InstancePtr->BaseAddr,
								XSP_DRR_OFFSET);
				if (DataWidth == XSP_DATAWIDTH_BYTE) {
					if(InstancePtr->RecvBufferPtr != NULL) {
						*InstancePtr->RecvBufferPtr++ =
							(u8)Data;
					}
				} else if (DataWidth ==
						XSP_DATAWIDTH_HALF_WORD) {
					if (InstancePtr->RecvBufferPtr != NULL){
					    *(u16 *)InstancePtr->RecvBufferPtr =
							(u16)Data;
						InstancePtr->RecvBufferPtr += 2;
					}
				} else if (DataWidth == XSP_DATAWIDTH_WORD) {
					if (InstancePtr->RecvBufferPtr != NULL){
					    *(u32 *)InstancePtr->RecvBufferPtr =
							Data;
						InstancePtr->RecvBufferPtr += 4;
					}
				}
				InstancePtr->Stats.BytesTransferred +=
						(DataWidth >> 3);
				ByteCount -= (DataWidth >> 3);
				StatusReg = XSpi_GetStatusReg(InstancePtr);
			}

			if (InstancePtr->RemainingBytes > 0) {

				StatusReg = XSpi_GetStatusReg(InstancePtr);

				while(((StatusReg & XSP_SR_TX_FULL_MASK)== 0) &&
					(InstancePtr->RemainingBytes > 0)) {
					if (DataWidth == XSP_DATAWIDTH_BYTE) {
						Data = *InstancePtr->
								SendBufferPtr;

					} else if (DataWidth ==
						XSP_DATAWIDTH_HALF_WORD) {

						Data = *(u16 *)InstancePtr->
								SendBufferPtr;
					} else if (DataWidth ==
							XSP_DATAWIDTH_WORD) {
						Data = *(u32 *)InstancePtr->
								SendBufferPtr;
					}
					XSpi_WriteReg(InstancePtr->BaseAddr,
							XSP_DTR_OFFSET, Data);
					InstancePtr->SendBufferPtr +=
							(DataWidth >> 3);
					InstancePtr->RemainingBytes -=
							(DataWidth >> 3);
					StatusReg = XSpi_GetStatusReg(
							InstancePtr);
				}

				ControlReg = XSpi_GetControlReg(InstancePtr);
				ControlReg &= ~XSP_CR_TRANS_INHIBIT_MASK;
				XSpi_SetControlReg(InstancePtr, ControlReg);
			}
		}

		ControlReg = XSpi_GetControlReg(InstancePtr);
		XSpi_SetControlReg(InstancePtr,
				    ControlReg | XSP_CR_TRANS_INHIBIT_MASK);

		XSpi_SetSlaveSelectReg(InstancePtr,
					InstancePtr->SlaveSelectMask);
		InstancePtr->IsBusy = FALSE;
	}

	return XST_SUCCESS;
}

int XSpi_Stop(XSpi *InstancePtr)
{

	printf("Xspi_stop\n");
	u32 ControlReg;

	if (InstancePtr->IsBusy) {
		return XST_DEVICE_BUSY;
	}

	XSpi_IntrGlobalDisable(InstancePtr);

	ControlReg = XSpi_GetControlReg(InstancePtr);
	XSpi_SetControlReg(InstancePtr, ControlReg & ~XSP_CR_ENABLE_MASK);

	InstancePtr->IsStarted = 0;

	return XST_SUCCESS;
}

void Run()
{
	printf("Run\n");
   	CAN_Message TxMessage;
   	u8 status;

//  xil_printf("Welcome to the PmodCAN IP Core Transmit Demo\r\n");

   	while (1) {

//  xil_printf("Waiting to send\r\n");
	printf("Waiting to send\n");
      do {
         status = CAN_ReadStatus(&myDevice);
      } while ((status & CAN_STATUS_TX0REQ_MASK) != 0); // Wait for buffer 0 to
                                                        // be clear

    TxMessage = ComposeMessage();

//  xil_printf("sending ");
	printf("sending");

    PrintMessage(TxMessage);

    CAN_ModifyReg(&myDevice, CAN_CANINTF_REG_ADDR, CAN_CANINTF_TX0IF_MASK, 0);

//  xil_printf("requesting to transmit message through transmit buffer 0 \r\n");
	printf("requesting to transmit message through transmit buffer 0\n");

    CAN_SendMessage(&myDevice, TxMessage, CAN_Tx0);

    CAN_ModifyReg(&myDevice, CAN_CANINTF_REG_ADDR, CAN_CANINTF_TX0IF_MASK, 0);

    do {
       	status = CAN_ReadStatus(&myDevice);
//      xil_printf("Waiting to complete transmission\r\n");
		printf("Waiting to complete transmission\n");
    } while ((status & CAN_STATUS_TX0IF_MASK) != 0); // Wait for message to
                                                       // transmit successfully

     sleep(1);
   }
}


void Cleanup()
{
	printf("Cleanup\n");
	   CAN_end(&myDevice);
}


void PrintMessage(CAN_Message message)
{
	printf("PrintMessage\n");
  u8 i;

   //xil_printf("message:\r\n");
	printf("mesage:\n");

//   xil_printf("    %s Frame\r\n", (message.ide) ? "Extended" : "Standard");
//   xil_printf("    ID: %03x\r\n", message.id);

	printf("	%s Frame\n", (message.ide) ? "Extended" : "Standart");
	printf("	ID: %03x\n", message.id);


   if (message.ide)
//      xil_printf("    EID: %05x\r\n", message.eid);
		printf("	EID: %05x\n", message.ide);

   if (message.rtr)
//      xil_printf("    Remote Transmit Request\r\n");	
		printf("	Remote Transmit Request\n");

   else
//      xil_printf("    Standard Data Frame\r\n");
		printf("	Standard Transmit Request\n");

//   xil_printf("    dlc: %01x\r\n", message.dlc);
//   xil_printf("    data:\r\n");

	printf("	dlc: %01x\n", message.dlc);
	printf("	data:\n");

   for (i = 0; i < message.dlc; i++)
//      xil_printf("        %02x\r\n", message.data[i]);
		printf("	%02x\n", message.data[i]);
}

CAN_Message ComposeMessage()
{
	printf("ComposeMessage\n");
   CAN_Message message;

   message.id = 0x2;
   message.dlc = 0x08;
   message.eid = 0x0;
   message.rtr = 0;
   message.ide = 0x00;
   message.data[0] = 0x01;
   message.data[1] = 0x02;
   message.data[2] = 0x04;
   message.data[3] = 0x08;
   message.data[4] = 0x10;
   message.data[5] = 0x20;
   message.data[6] = 0x40;
   message.data[7] = 0x80;

   return message;
}
