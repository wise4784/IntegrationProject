#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#include <stdint.h>
#include <stdbool.h>
#include <linux/spi/spidev.h>

/* GPIO */
#define IN			0
#define OUT			1

#define GPIO_MAP_SIZE		0x10000
#define GPIO_DATA_OFFSET	0x00
#define GPIO_TRI_OFFSET		0x04

/* LIDAR */
#define I2C_DEV	    "/dev/i2c-0"

#define LIDAR_ADDR    0x62

#define ACQ_COMMAND	    0x00
#define STATUS		    0x01
#define SIG_COUNT_VAL	    0x02
#define ACQ_CONFIG_REG	    0x04
#define FULL_DELAY_HIGH	    0x0f

#define OUTER_LOOP_COUNT    0x11
#define REF_COUNT_VAL	    0x12

#define THRESHOLD_BYPASS    0x1c
#define I2C_CONFIG	    0x1e

#define MEASURE_DELAY	    0x45

#define POWER_CONTROL	    0x65

#define READ_FROM	    0x89 // 0x8f?

#define NO_CORRECTION	    0
#define CORRECTION	    1

#define AR_VELOCITY	    0
#define	AR_PEAK_CORR	    1
#define	AR_NOISE_PEAK	    2
#define AR_SIGNAL_STRENGTH  3
#define AR_FULL_DELAY_HIGH  4
#define AR_FULL_DELAY_LOW   5

#define OUTPUT_OF_ALL	    0
#define DISTANCE_ONLY	    1
#define DISTANCE_WITH_VEL   2
#define VELOCITY_ONLY	    3

#define USAGE		    "exe 1\n"

/* Shared M */
#define KEY_PMOD1	1234
#define KEY_PMOD2	1235
#define KEY_LIDAR	1236
#define KEY_INPUT	1237
#define KEY_VALUE1	1238
#define KEY_VALUE2	1239

/* PMOD */
#define SPI_DEV "/dev/spidev1.0"

#define mcp25625_FAIL 	 0
#define mcp25625_SUCCESS 1

#define MCP_ALLTXBUSY	2
#define TIMEOUTVALUE	100

#define MCP_SIDH		0
#define MCP_SIDL		1
#define MCP_EID8		2
#define MCP_EID0		3

#define CAN_FAIL			0
#define CAN_OK				1
#define CAN_ERROR			2

#define CAN_GETTXBFTIMEOUT	6
#define CAN_SENDMSGTIMEOUT	7

//CANCTRL register values
#define MCP_NORMAL		0x00
#define MCP_SLEEP		0x20
#define MCP_LOOPBACK	0x40
#define MCP_LISTENONLY	0x60
#define MCP_CONFIG		0x80
#define MCP_POWERUP		0xE0
#define MODE_MASK		0xE0

#define MCP_RXB_RX_ANY	0x60
#define MCP_RXB_RX_EXT	0x40
#define MCP_RXB_RX_STD	0x20
#define MCP_RXB_RX_STDEXT	0x00
#define MCP_RXB_RX_MASK	0x60
#define MCP_RXB_BUKT_MASK	(1<<2)

#define MCP_STDEXT	0
#define MCP_STD		1
#define MCP_EXT		2
#define MCP_ANY		3

//crystal
#define MCP_20MHZ	0
#define MCP_16MHZ	1
#define MCP_8MHZ	2

//bitrate
#define CAN_40KBPS	0
#define CAN_80KBPS	1
#define CAN_100KBPS	2
#define CAN_200KBPS	3
#define CAN_250KBPS 4
#define CAN_500KBPS	5

//MCP25625 register addresses
#define MCP_RXF0SIDH    0x00
#define MCP_RXF0SIDL    0x01
#define MCP_RXF0EID8    0x02
#define MCP_RXF0EID0    0x03
#define MCP_RXF1SIDH    0x04
#define MCP_RXF1SIDL    0x05
#define MCP_RXF1EID8    0x06
#define MCP_RXF1EID0    0x07
#define MCP_RXF2SIDH    0x08
#define MCP_RXF2SIDL    0x09
#define MCP_RXF2EID8    0x0A
#define MCP_RXF2EID0    0x0B
#define MCP_CANSTAT     0x0E
#define MCP_CANCTRL     0x0F
#define MCP_RXF3SIDH    0x10
#define MCP_RXF3SIDL    0x11
#define MCP_RXF3EID8    0x12
#define MCP_RXF3EID0    0x13
#define MCP_RXF4SIDH    0x14
#define MCP_RXF4SIDL    0x15
#define MCP_RXF4EID8    0x16
#define MCP_RXF4EID0    0x17
#define MCP_RXF5SIDH    0x18
#define MCP_RXF5SIDL    0x19
#define MCP_RXF5EID8    0x1A
#define MCP_RXF5EID0    0x1B
#define MCP_TEC            0x1C
#define MCP_REC            0x1D
#define MCP_RXM0SIDH    0x20
#define MCP_RXM0SIDL    0x21
#define MCP_RXM0EID8    0x22
#define MCP_RXM0EID0    0x23
#define MCP_RXM1SIDH    0x24
#define MCP_RXM1SIDL    0x25
#define MCP_RXM1EID8    0x26
#define MCP_RXM1EID0    0x27
#define MCP_CNF3        0x28
#define MCP_CNF2        0x29
#define MCP_CNF1        0x2A
#define MCP_CANINTE        0x2B
#define MCP_CANINTF        0x2C
#define MCP_EFLG        0x2D
#define MCP_TXB0CTRL    0x30
#define MCP_TXB0SIDH	0x31
#define MCP_TXB0D0	0x36
#define MCP_TXB1CTRL    0x40
#define MCP_TXB1SIDH	0x41
#define MCP_TXB1D0	0x46
#define MCP_TXB2CTRL    0x50
#define MCP_TXB2SIDH	0x51
#define MCP_TXB2D0	0x56
#define MCP_RXB0CTRL    0x60
#define MCP_RXB0SIDH    0x61
#define MCP_RXB1CTRL    0x70
#define MCP_RXB1SIDH    0x71

#define MCP_RXBUF_0		MCP_RXB0SIDH
#define MCP_RXBUF_1		MCP_RXB1SIDH

#define MCP_TX_INT          0x1C       
#define MCP_TX01_INT        0x0C       
#define MCP_RX_INT          0x03       
#define MCP_NO_INT          0x00       
#define MCP_TX01_MASK       0x14
#define MCP_TX_MASK        0x54

#define MCP_TXB_TXREQ_M		0x08
#define MCP_RTR_MASK		0x40
#define MCP_RX_RTR			0x08
#define MCP_N_TXBUFFERS		3

#define MCP_STAT_RXIF_MASK	(0x03)
#define MCP_STAT_RX0IF		(1<<0)
#define MCP_STAT_RX1IF		(1<<1)
#define MCP_RXB_EXIDE_M		0x08

//SPI instruction set
#define MCP_RESET	0xC0
#define MCP_READ	0x03
#define MCP_WRITE	0x02
#define MCP_LD_TXBUF	0x40
#define MCP_READ_RX_BUFFER	0x90
#define MCP_READ_STATUS	0xA0
#define MCP_RX_STATUS	0xB0
#define MCP_BITMOD	0x05

//speed 8M
#define MCP_8MHz_500kBPS_CFG1 (0x00)
#define MCP_8MHz_500kBPS_CFG2 (0x90)
#define MCP_8MHz_500kBPS_CFG3 (0x82)

#define MCP_8MHz_250kBPS_CFG1 (0x00)
#define MCP_8MHz_250kBPS_CFG2 (0xB1)
#define MCP_8MHz_250kBPS_CFG3 (0x85)

#define MCP_8MHz_200kBPS_CFG1 (0x00)
#define MCP_8MHz_200kBPS_CFG2 (0xB4)
#define MCP_8MHz_200kBPS_CFG3 (0x86)

#define MCP_8MHz_100kBPS_CFG1 (0x01)
#define MCP_8MHz_100kBPS_CFG2 (0xB4)
#define MCP_8MHz_100kBPS_CFG3 (0x86)

#define MCP_8MHz_80kBPS_CFG1 (0x01)
#define MCP_8MHz_80kBPS_CFG2 (0xBF)
#define MCP_8MHz_80kBPS_CFG3 (0x87)

#define MCP_8MHz_40kBPS_CFG1 (0x03)
#define MCP_8MHz_40kBPS_CFG2 (0xBF)
#define MCP_8MHz_40kBPS_CFG3 (0x87)

// speed 16M
#define MCP_16MHz_500kBPS_CFG1 (0x00)
#define MCP_16MHz_500kBPS_CFG2 (0xF0)
#define MCP_16MHz_500kBPS_CFG3 (0x86)

#define MCP_16MHz_250kBPS_CFG1 (0x41)
#define MCP_16MHz_250kBPS_CFG2 (0xF1)
#define MCP_16MHz_250kBPS_CFG3 (0x85)

#define MCP_16MHz_200kBPS_CFG1 (0x01)
#define MCP_16MHz_200kBPS_CFG2 (0xFA)
#define MCP_16MHz_200kBPS_CFG3 (0x87)

#define MCP_16MHz_100kBPS_CFG1 (0x03)
#define MCP_16MHz_100kBPS_CFG2 (0xFA)
#define MCP_16MHz_100kBPS_CFG3 (0x87)

#define MCP_16MHz_80kBPS_CFG1 (0x03)
#define MCP_16MHz_80kBPS_CFG2 (0xFF)
#define MCP_16MHz_80kBPS_CFG3 (0x87)

#define MCP_16MHz_40kBPS_CFG1 (0x07)
#define MCP_16MHz_40kBPS_CFG2 (0xFF)
#define MCP_16MHz_40kBPS_CFG3 (0x87)

//  speed 20M
#define MCP_20MHz_500kBPS_CFG1 (0x00)
#define MCP_20MHz_500kBPS_CFG2 (0xFA)
#define MCP_20MHz_500kBPS_CFG3 (0x87)

#define MCP_20MHz_250kBPS_CFG1 (0x41)
#define MCP_20MHz_250kBPS_CFG2 (0xFB)
#define MCP_20MHz_250kBPS_CFG3 (0x86)

#define MCP_20MHz_200kBPS_CFG1 (0x01)
#define MCP_20MHz_200kBPS_CFG2 (0xFF)
#define MCP_20MHz_200kBPS_CFG3 (0x87)

#define MCP_20MHz_100kBPS_CFG1 (0x04)
#define MCP_20MHz_100kBPS_CFG2 (0xFA)
#define MCP_20MHz_100kBPS_CFG3 (0x87)

#define MCP_20MHz_80kBPS_CFG1 (0x04)
#define MCP_20MHz_80kBPS_CFG2 (0xFF)
#define MCP_20MHz_80kBPS_CFG3 (0x87)

#define MCP_20MHz_40kBPS_CFG1 (0x09)
#define MCP_20MHz_40kBPS_CFG2 (0xFF)
#define MCP_20MHz_40kBPS_CFG3 (0x87)


typedef struct _SPI_RX{
	uint16_t ID;
	uint32_t EID;
	uint8_t  IDE;
	uint8_t  DLC;
	uint8_t  RTR;
	uint8_t  Buf[8];
	uint8_t ReceiveStatus;
}SPI_RX;

typedef struct _SPI_TX{
	uint16_t ID;
	uint32_t EID;
	uint8_t IDE;
	uint8_t RTR;
	uint8_t DLC;
	uint8_t Buf[8];
	uint8_t TXREQ;
}SPI_TX;


void usage(void);

/* Lidar Func */
unsigned get_status(void);
void lidar_read(unsigned char, unsigned, unsigned char *);
void lidar_write(unsigned char, unsigned char);
void measurement(unsigned char, unsigned char, unsigned char *);
void display(unsigned char, unsigned char *);


/* SPI Func */
static void dumpstat(const char *name, int fd);
static void do_msg(int fd, unsigned char *buf, int len);

bool mcp25625_init(int fd, const uint8_t canIDMode, const uint8_t canSpeed, const uint8_t canClock);
void mcp25625_reset(int fd);
bool mcp25625_setCANCTRL_Mode(int fd, uint8_t mcpMode);
bool mcp25625_configRate(int fd, const uint8_t canSpeed, const uint8_t canClock);
void mcp25625_initCANBuffers(int fd);
void mcp25625_modifyRegister(int fd, const uint8_t address, const uint8_t mask, const uint8_t data);
uint8_t mcp25625_readRegister(int fd, const uint8_t address);
void mcp25625_setRegister(int fd, const uint8_t address, const uint8_t value);
uint8_t mcp25625_getNextFreeTXBuf(int fd, uint8_t *tx_sidh);
void mcp25625_write_canMsg(int fd, const uint8_t buffer_sidh_addr);
void mcp25625_write_id(int fd, unsigned char *rts, const uint8_t mcp_addr);
void mcp25625_write_data(int fd, const uint8_t mcp_addr);
void mcp25625_loadTXBuffer(int fd, unsigned char * rts, unsigned char * txbuf, const uint8_t buffer_addr);
void mcp25625_setMsg(int fd, unsigned char * rts, unsigned char * txbuf, const uint8_t buffer_addr);

uint8_t sendMsgBuf(int fd, uint16_t id, uint32_t eid, uint8_t ide, uint8_t rtr, uint8_t len, uint8_t *data);
void setMsg(int fd, uint16_t id, uint32_t eid, uint8_t ide, uint8_t rtr, uint8_t len, uint8_t * data);
uint8_t sendMsg(int fd);


uint8_t readMsgBuf(int fd, uint16_t *id, uint8_t *len, unsigned char *rxBuf, uint8_t RXxIF);
uint8_t readMsg(int fd, uint8_t RXxIF);
uint8_t mcp25625_readStatus(int fd);
void mcp25625_read_canMsg(int fd, const uint8_t buffer_sidh_addr, uint8_t nm);
void mcp25625_read_buf(int fd, const uint8_t mcp_addr, uint8_t *ext, uint16_t *id, uint8_t nm);

void mcp25625_ReadRXBuffer(int fd, unsigned char * rxbuf, const uint8_t buffer_addr, uint8_t nm);

void WAIT(int delay);

/* SPI DATA */
SPI_TX TX = {0, 0x2, 1, 0, 0x8, {0,},0};
SPI_RX RX;
int fd_spi;
uint8_t tx_sidh;

/* LIDAR DATA */
int fd_lidar;
unsigned char receives[8] = {AR_VELOCITY, 0, 0,AR_PEAK_CORR, AR_NOISE_PEAK, AR_SIGNAL_STRENGTH, AR_FULL_DELAY_HIGH, AR_FULL_DELAY_LOW};
unsigned char options = 1;
char *fname;	
int Lidar_Value;

int main(void)
{
/* SPI DATA */
	const char * pname = SPI_DEV;	
	uint8_t rx_value=0;

/* FORK DATA*/
	int N = 3;
	pid_t pid[N];
	int i;

/* LIDAR DATA */
	uint8_t LidarCount=0;

/*  INPUT DATA */
	int fd_input;
	int direction = IN;
	char *uiod;
	int value_input = 0;
	void *ptr;

/* SEMA DATA */
	sem_t *lidarToPmodFlag;
	sem_t *inputToPmodFlag;
	sem_t *pmodToLidarFlag;
	sem_t *pmodToInputFlag;


/* SHM DATA */
	char *flag_PmodToLidar;
	char *flag_PmodToInput;
	char *flag_LidarToPmod;
	char *flag_InputToPmod;
	int *InputValue;
	int *LidarValue;
	
	int shmid_PmodToLidar;
	int shmid_PmodToInput;
	int shmid_LidarToPmod;
	int shmid_InputToPmod;
	int shmid_InputValue;
	int shmid_LidarValue;

	void *shared_memory_PmodToLidar = (void *)0;
	void *shared_memory_PmodToInput = (void *)0;
	void *shared_memory_LidarToPmod = (void *)0;
	void *shared_memory_InputToPmod = (void *)0;
	void *shared_memory_InputValue = (void *)0;
	void *shared_memory_LidarValue = (void *)0;

/*
	shmid_PmodToLidar = shmget((key_t)KEY_PMOD1,sizeof(char),0666|IPC_CREAT);
	shmid_PmodToInput = shmget((key_t)KEY_PMOD2,sizeof(char),0666|IPC_CREAT);
	shmid_LidarToPmod = shmget((key_t)KEY_LIDAR,sizeof(char),0666|IPC_CREAT);
	shmid_InputToPmod = shmget((key_t)KEY_INPUT,sizeof(char),0666|IPC_CREAT);
	shmid_LidarValue = shmget((key_t)KEY_VALUE1,sizeof(int),0666|IPC_CREAT);
	shmid_InputValue = shmget((key_t)KEY_VALUE2,sizeof(int),0666|IPC_CREAT);

	shared_memory_PmodToLidar = shmat(shmid_PmodToLidar, (void *)0, 0);
	shared_memory_PmodToInput = shmat(shmid_PmodToInput, (void *)0, 0);
	shared_memory_LidarToPmod = shmat(shmid_LidarToPmod, (void *)0, 0);
	shared_memory_InputToPmod = shmat(shmid_InputToPmod, (void *)0, 0);
	shared_memory_InputValue = shmat(shmid_LidarValue, (void *)0, 0);
	shared_memory_LidarValue = shmat(shmid_InputValue, (void *)0, 0);

	flag_PmodToLidar = (char *)shared_memory_PmodToLidar;
	flag_PmodToInput = (char *)shared_memory_PmodToInput;
	flag_LidarToPmod = (char *)shared_memory_LidarToPmod;
	flag_InputToPmod = (char *)shared_memory_InputToPmod;
	InputValue = (int *)shared_memory_InputValue;
	LidarValue = (int *)shared_memory_LidarValue;
*/
	

	for(i = 0; i < N; i++)
	{
		pid[i] = fork();
		
	}

	if(pid[0] == 0)
	{	
		if( -1 == (shmid_PmodToLidar = shmget( (key_t)KEY_PMOD1,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_PmodToLidar generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_PmodToLidar = shmat(shmid_PmodToLidar, (void *)0 , 0)))
		{
			printf("shm_PmodToLidar reference failed\n");
			return -1;
		}

		if( -1 == (shmid_LidarToPmod = shmget( (key_t)KEY_LIDAR,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_LidarToPmod generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_LidarToPmod = shmat(shmid_LidarToPmod, (void *)0 , 0)))
		{
			printf("shm_LidarToPmod reference failed\n");
			return -1;
		}

		if( -1 == (shmid_LidarValue = shmget( (key_t)KEY_VALUE1,sizeof(int), IPC_CREAT | 0666)))
		{
			printf("shm_LidarValue generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_LidarValue = shmat(shmid_LidarValue, (void *)0 , 0)))
		{
			printf("shm_LidarValue reference failed\n");
			return -1;
		}

		flag_PmodToLidar = (char *)shared_memory_PmodToLidar;
		flag_LidarToPmod = (char *)shared_memory_LidarToPmod;
		LidarValue = (int *)shared_memory_LidarValue;

		
		if((pmodToLidarFlag = sem_open("pmodToLidarFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}


		if((lidarToPmodFlag = sem_open("lidarToPmodFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}



		fname = "/dev/i2c-0";	
		fd_lidar = open(fname, O_RDWR);
	
		if(fd_lidar)
		{
			printf("Lidar Success\n");
		}
		else
		{
			printf("Lidar Fail\n");

		}

		if(ioctl(fd_lidar, I2C_SLAVE, LIDAR_ADDR) < 0)
		{
			perror("Slave Addr Connect Failed\n");
		}

		lidar_write(SIG_COUNT_VAL, 0x80);
		lidar_write(ACQ_CONFIG_REG, 0x08);
		lidar_write(THRESHOLD_BYPASS, 0x00);


		while(1)
		{
//			sem_wait(pmodToLidarFlag);
			if(*flag_PmodToLidar)
			{
				printf("Lidar Receive Signal\n");
				LidarCount++;
				if(LidarCount == 99)
				{
					LidarCount = 0;
					measurement(CORRECTION, options, receives);
					if(!Lidar_Value)
					{
						measurement(CORRECTION, options, receives);
					}

					*LidarValue = Lidar_Value;
					printf("LidarCount=%d\n",LidarCount);
					usleep(3700);
				}
				else
				{
					measurement(NO_CORRECTION, options, receives);
					if(!Lidar_Value)
					{
						measurement(CORRECTION, options, receives);
					}

					*LidarValue = Lidar_Value;
					printf("Lidar Count !=99\n");
					usleep(3700);
				}
			
				printf("LLLLLLLLLLLLLLL VALUE : %d\n",*LidarValue);

				*flag_PmodToLidar = 0;
				//sem_wait(lidarToPmodFlag);
				*flag_LidarToPmod = 1;
				//sem_post(lidarToPmodFlag);
				printf("Lidar TO Pmod Signal=%d\n",*flag_PmodToLidar);
			}
//			sem_post(pmodToLidarFlag);
		}

		close(fd_lidar);	
	}

	if(pid[1] == 0)
	{
	
		if( -1 == (shmid_PmodToInput = shmget( (key_t)KEY_PMOD2,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_PmodToLidar generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_PmodToInput = shmat(shmid_PmodToInput, (void *)0 , 0)))
		{
			printf("shm_PmodToLidar reference failed\n");
			return -1;
		}

		if( -1 == (shmid_InputToPmod = shmget( (key_t)KEY_INPUT,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_LidarToPmod generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_InputToPmod = shmat(shmid_InputToPmod, (void *)0 , 0)))
		{
			printf("shm_LidarToPmod reference failed\n");
			return -1;
		}

		if( -1 == (shmid_InputValue = shmget( (key_t)KEY_VALUE2,sizeof(int), IPC_CREAT | 0666)))
		{
			printf("shm_LidarValue generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_InputValue = shmat(shmid_InputValue, (void *)0 , 0)))
		{
			printf("shm_LidarValue reference failed\n");
			return -1;
		}

		flag_PmodToInput = (char *)shared_memory_PmodToInput;
		flag_InputToPmod = (char *)shared_memory_InputToPmod;
		InputValue = (int *)shared_memory_InputValue;


		if((pmodToInputFlag = sem_open("pmodToInputFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}

		if((inputToPmodFlag = sem_open("inputToPmodFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}



		uiod = "/dev/uio0";
		fd_input = open(uiod,O_RDWR);

		if(fd_input)
		{
			printf("Encoder Success\n");
		}		
		else
		{
			printf("Encoder Fail\n");
			usage();
		}
/*
		if(fd_input < 1)
		{
			printf("Invalid UIO device file : %s.\n",uiod);
			usage();
			return -1;
		}			
*/
		printf("Input Success\n");
		ptr = mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,fd_input,0);
		*((unsigned *)(ptr + GPIO_TRI_OFFSET)) = 1023;	
		while(1)
		{

//			sem_wait(pmodToInputFlag);
			if(*flag_PmodToInput)
			{
				printf(" Encoder Receive Signal\n");
				*InputValue = *((unsigned *)(ptr + GPIO_DATA_OFFSET));
				// *InputValue = value_input;
				printf("EEEE  InputValue = %d\n",*InputValue);
				//sem_wait(inputToPmodFlag);
				*flag_InputToPmod = 1;
				//sem_post(inputToPmodFlag);

				*flag_PmodToInput = 0;
				printf("Encoder To Pmod Signal\n");
			}
//			sem_post(pmodToInputFlag);
		}
		munmap(ptr,GPIO_MAP_SIZE);
		close(fd_input);
	}

	if(pid[2] == 0)
	{
		if( -1 == (shmid_PmodToLidar = shmget( (key_t)KEY_PMOD1,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_PmodToLidar generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_PmodToLidar = shmat(shmid_PmodToLidar, (void *)0 , 0)))
		{
			printf("shm_PmodToLidar reference failed\n");
			return -1;
		}

		if( -1 == (shmid_LidarToPmod = shmget( (key_t)KEY_LIDAR,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_LidarToPmod generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_LidarToPmod = shmat(shmid_LidarToPmod, (void *)0 , 0)))
		{
			printf("shm_LidarToPmod reference failed\n");
			return -1;
		}

		if( -1 == (shmid_LidarValue = shmget( (key_t)KEY_VALUE1,sizeof(int), IPC_CREAT | 0666)))
		{
			printf("shm_LidarValue generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_LidarValue = shmat(shmid_LidarValue, (void *)0 , 0)))
		{
			printf("shm_LidarValue reference failed\n");
			return -1;
		}
		if( -1 == (shmid_PmodToInput = shmget( (key_t)KEY_PMOD2,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_PmodToLidar generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_PmodToInput = shmat(shmid_PmodToInput, (void *)0 , 0)))
		{
			printf("shm_PmodToLidar reference failed\n");
			return -1;
		}

		if( -1 == (shmid_InputToPmod = shmget( (key_t)KEY_INPUT,sizeof(char), IPC_CREAT | 0666)))
		{
			printf("shm_LidarToPmod generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_InputToPmod = shmat(shmid_InputToPmod, (void *)0 , 0)))
		{
			printf("shm_LidarToPmod reference failed\n");
			return -1;
		}

		if( -1 == (shmid_InputValue = shmget( (key_t)KEY_VALUE2,sizeof(int), IPC_CREAT | 0666)))
		{
			printf("shm_LidarValue generation failed\n");
			return -1;
		}
		if( (void *)-1 == (shared_memory_InputValue = shmat(shmid_InputValue, (void *)0 , 0)))
		{
			printf("shm_LidarValue reference failed\n");
			return -1;
		}

	 	flag_PmodToLidar = (char *)shared_memory_PmodToLidar;
		flag_PmodToInput = (char *)shared_memory_PmodToInput;
		flag_LidarToPmod = (char *)shared_memory_LidarToPmod;
		flag_InputToPmod = (char *)shared_memory_InputToPmod;
		InputValue = (int *)shared_memory_InputValue;
		LidarValue = (int *)shared_memory_LidarValue;

		*flag_PmodToLidar = 0;
		*flag_PmodToInput = 0;
		*flag_LidarToPmod = 0;
		*flag_InputToPmod = 0;

		sem_unlink("lidarToPmodFlag");
		sem_unlink("inputToPmodFlag");
		sem_unlink("pmodToLidarFlag");
		sem_unlink("pmodToInputFlag");


		if((pmodToLidarFlag = sem_open("pmodToLidarFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}

		if((pmodToInputFlag = sem_open("pmodToInputFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}
	
		if((lidarToPmodFlag = sem_open("lidarToPmodFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}

		if((inputToPmodFlag = sem_open("inputToPmodFlag",O_CREAT,0777,1)) == NULL)
		{
			perror("SEMA OPEN ERROR\n");	
			return -1;
		}


		//Spi Open
		if((fd_spi = open(pname, O_RDWR))<0)
		{
			perror("Spi Open Success\n");
			return -1;
		}
		printf("Spi Open Success\n");
		
		//Spi stat verification
		dumpstat(pname, fd_spi);
	
		//Spi init
		if(mcp25625_init(fd_spi, MCP_ANY, CAN_250KBPS, MCP_20MHZ) == mcp25625_SUCCESS)
			printf("MCP25625 Initialized Successfully!\n");
		else
		{
			perror("MCP25625 Initializing Error\n");
			return -1;
		}

		while(1)
		{
			RX.ReceiveStatus = mcp25625_readStatus(fd_spi);	
			RX.ReceiveStatus &= MCP_STAT_RXIF_MASK;


			if((RX.ReceiveStatus & MCP_STAT_RX0IF) == MCP_STAT_RX0IF)
			{
				mcp25625_modifyRegister(fd_spi, tx_sidh-1, 0x08,0);
				printf("Receive SUCCESS RX0IF= %d\n",RX.ReceiveStatus);
			
				if(readMsgBuf(fd_spi, &RX.ID, &RX.DLC, RX.Buf, MCP_STAT_RX0IF))
				{
					usleep(100);
					rx_value = RX.Buf[7];
					if(rx_value == 0x46)
					{
						printf("rx_value\n");
						if( (RX.IDE & 0x80000000) == 0x80000000 )
						{
							printf("Extended ID : 0x%x  DLC : %1d Data\n ",RX.ID, RX.DLC);
						}
						else
						{
							printf("Standard ID : 0x%x DLC : %1d Data\n", RX.ID, RX.DLC);
						}

			
						//sem_wait(pmodToInputFlag);
						*flag_PmodToInput = 1;
						//sem_post(pmodToInputFlag);
	
						//sem_wait(pmodToLidarFlag);
						*flag_PmodToLidar = 1;
						//sem_post(pmodToLidarFlag);
		
						//printf("Pmod : flag_PmodToInput = %d\n",*flag_PmodToInput);
						//printf("Pmod : flag_PmodToLidar = %d\n",*flag_PmodToLidar);
						
						usleep(1000);
						while( !(*flag_LidarToPmod) || !(*flag_InputToPmod)   )
						{
							//printf("flag_LidarToPmod= %d\n",*flag_LidarToPmod);
							//printf("flag_InputToPmod= %d\n",*flag_InputToPmod);
							usleep(1000);
						}
	//TX				
						//printf("flag_LidarToPmod=%d\n",*flag_LidarToPmod);
						//printf("flag_InputToPmod=%d\n",*flag_InputToPmod);
						//sem_wait(lidarToPmodFlag);
						TX.Buf[0] = (uint8_t)( (*LidarValue) & 0x0FF);
						TX.Buf[1] = (uint8_t)( ( (*LidarValue) >> 8) & 0x0F );
						*flag_LidarToPmod = 0;
						//sem_post(inputToPmodFlag);

						//sem_wait(inputToPmodFlag);
						TX.Buf[6] = (uint8_t)( (*InputValue) & 0x0FF);
						TX.Buf[7] = (uint8_t)( ( (*InputValue) >> 8) & 0x03 );	
						*flag_InputToPmod = 0;
						*InputValue = 0;
						//sem_post(inputToPmodFlag);	
				
						sendMsgBuf(fd_spi, TX.ID, TX.EID, TX.IDE, TX.RTR, TX.DLC, TX.Buf);
						mcp25625_modifyRegister(fd_spi, tx_sidh-1, 0x08,0);	
						printf("Lidar =%d\n",TX.Buf[0] |  ( (uint16_t)(TX.Buf[1]) << 8)  );
						printf("Input =%d\n",TX.Buf[6] | ( (uint16_t)(TX.Buf[7]) << 8) );
			
						usleep(1000);

					}
				}

			}
			else if( (RX.ReceiveStatus & MCP_STAT_RX1IF) == MCP_STAT_RX1IF)
			{
				mcp25625_modifyRegister(fd_spi, tx_sidh-1, 0x08, 0);
				printf("Receive SUCCESS RX1IF=%d\n",RX.ReceiveStatus);
				readMsgBuf(fd_spi, &RX.ID, &RX.DLC, RX.Buf, MCP_STAT_RX1IF);

				if( (RX.IDE & 0x80000000) == 0x80000000)
				{
					printf("Extended ID : 0x%x DLC : %1d\n",RX.ID, RX.DLC);
				}
				else
				{
					printf("Standard ID : 0x%x DLC : %1d\n",RX.ID, RX.DLC);
				}
			}


		}
		close(fd_spi);

	}

    return 0;
}



void usage(void)
{
	printf(" *argv[0] -d <UIO_DEV_FILE> -i|-o <VALUE>\n");
	printf("	-d					UIO device file. e.g. /dev/uio0");
	printf("	-i					INPUT from GPIO\n");
	printf("	-o <VALUE>			OUTPUT to GPIO\n");

	return;
}



unsigned get_status(void)
{
    unsigned char buf[1] = {STATUS};

    if(write(fd_lidar, buf, 1) != 1)
    {
		perror("Write Reg Error");
		return -1;
    }
    if(read(fd_lidar, buf, 1) != 1)
    {
		perror("Read Reg Error");
		return -1;
    }

    return buf[0] & 0x01;
}

void lidar_read(unsigned char reg, unsigned read_size, unsigned char *receives)
{
    unsigned char buf[1] = {reg};
    unsigned busy_flag = 1;
    unsigned busy_counter = 0;

    while(busy_flag)
    {
		busy_flag = get_status();
		busy_counter++;

		if(busy_counter > 9999)
		{
		    printf("Busy Count Time Out!\n");
		}
    }

    if(!busy_flag)
    {
		if(write(fd_lidar, buf, 1) != 1)
		{
		    perror("Write Reg Error");
		}
	
		if(read(fd_lidar, receives, read_size) != read_size)
		{
		    perror("Read Reg Error");
		}
    }
}

void lidar_write(unsigned char reg, unsigned char val)
{
    unsigned char buf[2] = {reg, val};

    if(write(fd_lidar, buf, 2) != 2)
    {
		perror("Write Reg Error");
    }

    usleep(1000);
}

void measurement(unsigned char is_corr, unsigned char opt, unsigned char *buf)
{
    unsigned char i;

    if(is_corr)
		lidar_write(ACQ_COMMAND, 0x04);
    else
		lidar_write(ACQ_COMMAND, 0x03);

    lidar_read(READ_FROM, 8, buf);

    for(i = 1; i < 6; i++)
		buf[i] = buf[i + 2];

    display(opt, buf);
}

void display(unsigned char opt, unsigned char *buf)
{
    Lidar_Value = buf[AR_FULL_DELAY_HIGH] << 8 | buf[AR_FULL_DELAY_LOW];	

}




void WAIT(int delay)
{
	int i;

	for(i=0; i<delay; i++)
		;
}

//pmod CAN function 
bool mcp25625_init(int fd, const uint8_t canIDMode, const uint8_t canSpeed, const uint8_t canClock)
{
	uint8_t mcpMode, res;

	mcp25625_reset(fd);

	if(mcp25625_setCANCTRL_Mode(fd, MCP_CONFIG) == mcp25625_SUCCESS)
		printf("Entering Configuration Mode..\n");
	else
	{
		printf("Fail to Entering Configuration Mode..\n");
		return mcp25625_FAIL;
	}

	if(mcp25625_configRate(fd, canSpeed, canClock) == mcp25625_SUCCESS)
		printf("Bitrate Setting Success!!\n");
	else
	{
		printf("Fail to Setting Bitrate..\n");
		return mcp25625_FAIL;
	}

	/* RX Interrupt Enable */
	//

	mcp25625_initCANBuffers(fd);

	mcp25625_modifyRegister(fd, MCP_RXB0CTRL, MCP_RXB_RX_MASK, MCP_RXB_RX_ANY);
	mcp25625_modifyRegister(fd, MCP_RXB1CTRL, MCP_RXB_RX_MASK, MCP_RXB_RX_ANY);

	if(mcp25625_setCANCTRL_Mode(fd, MCP_NORMAL) == mcp25625_SUCCESS)
		printf("Entering to Normal Mode...!\n");
	else
	{
		printf("Fail to Entering Normal Mode..\n");
		return mcp25625_FAIL;
	}	
	
	return mcp25625_SUCCESS;	
}

void mcp25625_reset(int fd)
{
	unsigned char buf = MCP_RESET;

	do_msg(fd, &buf, 1);
}

bool mcp25625_setCANCTRL_Mode(int fd, uint8_t mcpMode)
{
	uint8_t n;

	mcp25625_modifyRegister(fd, MCP_CANCTRL, MODE_MASK, mcpMode);

	n = mcp25625_readRegister(fd, MCP_CANCTRL);
	n &= MODE_MASK;

	if(n == mcpMode)
		return mcp25625_SUCCESS;

	return mcp25625_FAIL;
}

bool mcp25625_configRate(int fd, const uint8_t canSpeed, const uint8_t canClock)
{
	uint8_t set, cfg1, cfg2, cfg3;
    set = 1;
    switch (canClock)
    {
        case (MCP_8MHZ):
        switch (canSpeed) 
        {
            case (CAN_40KBPS):                                              //  40Kbps
            cfg1 = MCP_8MHz_40kBPS_CFG1;
            cfg2 = MCP_8MHz_40kBPS_CFG2;
            cfg3 = MCP_8MHz_40kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cfg1 = MCP_8MHz_80kBPS_CFG1;
            cfg2 = MCP_8MHz_80kBPS_CFG2;
            cfg3 = MCP_8MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cfg1 = MCP_8MHz_100kBPS_CFG1;
            cfg2 = MCP_8MHz_100kBPS_CFG2;
            cfg3 = MCP_8MHz_100kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cfg1 = MCP_8MHz_200kBPS_CFG1;
            cfg2 = MCP_8MHz_200kBPS_CFG2;
            cfg3 = MCP_8MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cfg1 = MCP_8MHz_250kBPS_CFG1;
            cfg2 = MCP_8MHz_250kBPS_CFG2;
            cfg3 = MCP_8MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cfg1 = MCP_8MHz_500kBPS_CFG1;
            cfg2 = MCP_8MHz_500kBPS_CFG2;
            cfg3 = MCP_8MHz_500kBPS_CFG3;
            break;
        
            default:
            set = 0;
	    return mcp25625_FAIL;
            break;
        }
        break;

        case (MCP_16MHZ):
        switch (canSpeed) 
        {

            case (CAN_40KBPS):                                              //  40Kbps
            cfg1 = MCP_16MHz_40kBPS_CFG1;
            cfg2 = MCP_16MHz_40kBPS_CFG2;
            cfg3 = MCP_16MHz_40kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cfg1 = MCP_16MHz_80kBPS_CFG1;
            cfg2 = MCP_16MHz_80kBPS_CFG2;
            cfg3 = MCP_16MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cfg1 = MCP_16MHz_100kBPS_CFG1;
            cfg2 = MCP_16MHz_100kBPS_CFG2;
            cfg3 = MCP_16MHz_100kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cfg1 = MCP_16MHz_200kBPS_CFG1;
            cfg2 = MCP_16MHz_200kBPS_CFG2;
            cfg3 = MCP_16MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cfg1 = MCP_16MHz_250kBPS_CFG1;
            cfg2 = MCP_16MHz_250kBPS_CFG2;
            cfg3 = MCP_16MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cfg1 = MCP_16MHz_500kBPS_CFG1;
            cfg2 = MCP_16MHz_500kBPS_CFG2;
            cfg3 = MCP_16MHz_500kBPS_CFG3;
            break;

            default:
            set = 0;
	    return mcp25625_FAIL;
            break;
        }
        break;
        
        case (MCP_20MHZ):
        switch (canSpeed) 
        {
            case (CAN_40KBPS):                                              //  40Kbps
            cfg1 = MCP_20MHz_40kBPS_CFG1;
            cfg2 = MCP_20MHz_40kBPS_CFG2;
            cfg3 = MCP_20MHz_40kBPS_CFG3;
            break;

            case (CAN_80KBPS):                                              //  80Kbps
            cfg1 = MCP_20MHz_80kBPS_CFG1;
            cfg2 = MCP_20MHz_80kBPS_CFG2;
            cfg3 = MCP_20MHz_80kBPS_CFG3;
            break;

            case (CAN_100KBPS):                                             // 100Kbps
            cfg1 = MCP_20MHz_100kBPS_CFG1;
            cfg2 = MCP_20MHz_100kBPS_CFG2;
            cfg3 = MCP_20MHz_100kBPS_CFG3;
            break;

            case (CAN_200KBPS):                                             // 200Kbps
            cfg1 = MCP_20MHz_200kBPS_CFG1;
            cfg2 = MCP_20MHz_200kBPS_CFG2;
            cfg3 = MCP_20MHz_200kBPS_CFG3;
            break;

            case (CAN_250KBPS):                                             // 250Kbps
            cfg1 = MCP_20MHz_250kBPS_CFG1;
            cfg2 = MCP_20MHz_250kBPS_CFG2;
            cfg3 = MCP_20MHz_250kBPS_CFG3;
            break;

            case (CAN_500KBPS):                                             // 500Kbps
            cfg1 = MCP_20MHz_500kBPS_CFG1;
            cfg2 = MCP_20MHz_500kBPS_CFG2;
            cfg3 = MCP_20MHz_500kBPS_CFG3;
            break;

            default:
            set = 0;
            return mcp25625_FAIL;
            break;
        }
        break;
        
        default:
        set = 0;
		return mcp25625_FAIL;
        break;
    }

    if (set) {

        mcp25625_setRegister(fd, MCP_CNF3, cfg3);
	WAIT(100);

        mcp25625_setRegister(fd, MCP_CNF2, cfg2);
	WAIT(100);

        mcp25625_setRegister(fd, MCP_CNF1, cfg1);
	WAIT(100);
        return mcp25625_SUCCESS;
    }
     
    return mcp25625_FAIL;
}

void mcp25625_initCANBuffers(int fd)
{
	uint8_t i;
	unsigned char a1, a2, a3;

	a1 = MCP_RXF0SIDH;
	a2 = MCP_RXF3SIDH;

	for(i=0; i<9; i++)
	{
		mcp25625_setRegister(fd, a1, 0);
		mcp25625_setRegister(fd, a2, 0);
		a1++;
		a2++;
	}

	
	a1 = MCP_RXM0SIDH;

	for(i=0; i<5; i++)
	{
		mcp25625_setRegister(fd, a1, 0);
		a1++;
	}

	a1 = MCP_TXB0CTRL;
	a2 = MCP_TXB1CTRL;
	a3 = MCP_TXB2CTRL;

	for(i=0; i<11; i++)
	{
		mcp25625_setRegister(fd, a1, 0);
		mcp25625_setRegister(fd, a2, 0);
		mcp25625_setRegister(fd, a3, 0);
		a1++;
		a2++;
		a3++;
	}

	mcp25625_setRegister(fd, MCP_RXB0CTRL, 0);
	mcp25625_setRegister(fd, MCP_RXB1CTRL, 0);
}

void mcp25625_modifyRegister(int fd, const uint8_t address, const uint8_t mask, const uint8_t data)
{
	unsigned char buf[4];
	memset(buf, 0, sizeof(buf));
	
	buf[0] = MCP_BITMOD;
	buf[1] = address;
	buf[2] = mask;
	buf[3] = data;

	do_msg(fd, buf, sizeof(buf));

}

uint8_t mcp25625_readRegister(int fd, const uint8_t address)
{
	unsigned char buf[2];
	memset(buf, 0, sizeof(buf));

	buf[0] = MCP_READ;
	buf[1] = address;

	do_msg(fd, buf, sizeof(buf));

	return buf[0];	
}

void mcp25625_setRegister(int fd, const uint8_t address, const uint8_t value)
{
	unsigned char buf[3];
	memset(buf, 0, sizeof(buf));

	buf[0] = MCP_WRITE;
	buf[1] = address;
	buf[2] = value;

	do_msg(fd, buf, sizeof(buf));
	
}

uint8_t mcp25625_getNextFreeTXBuf(int fd, uint8_t *tx_sidh)
{
	
	uint8_t res, i, ctrlval;
	uint8_t ctrlregs[MCP_N_TXBUFFERS] = {MCP_TXB0CTRL, MCP_TXB1CTRL, MCP_TXB2CTRL};

	res = MCP_ALLTXBUSY;
	*tx_sidh = 0x00;

	for(i=0;i<MCP_N_TXBUFFERS;i++)
	{
		ctrlval = mcp25625_readRegister(fd, ctrlregs[i]);
		if((ctrlval & MCP_TXB_TXREQ_M)==0)
		{
			*tx_sidh = ctrlregs[i] + 1;
			res = mcp25625_SUCCESS;
			return res;
		} 
	}
	return res;
}

void mcp25625_write_data(int fd, const uint8_t mcp_addr)
{
//	printf("write_data\n");

	int i;
	uint8_t a1 = mcp_addr;
	unsigned char buf[3];
	memset(buf, 0, sizeof(buf));

	buf[0] = MCP_WRITE;
	for(i=0; i<TX.DLC; i++)
	{
		buf[1] = a1++;
		buf[2] = (unsigned char)TX.Buf[i];
		
		do_msg(fd, buf, sizeof(buf));
	}
}

uint8_t sendMsgBuf(int fd, uint16_t id, uint32_t eid, uint8_t ide, uint8_t rtr, uint8_t len, uint8_t * data)
{
//	printf("sendMsgBuf\n");
	uint8_t res;

	setMsg(fd, id, eid, ide, rtr, len, data);

	res = sendMsg(fd);

	return res;
}

void setMsg(int fd, uint16_t id, uint32_t eid, uint8_t ide, uint8_t rtr, uint8_t len, uint8_t * data)
{
	int i;

	TX.ID = id;
	TX.EID = eid;
	TX.IDE = ide;
	TX.RTR = rtr;
	TX.DLC = len;
	
	for(i=0; i<len; i++)
		TX.Buf[i] = data[i];

	printf("canID = %02x\n", TX.ID);
	printf("canEID = %02x\n", TX.EID);
	printf("canIDE = %02x\n", TX.IDE);
	printf("canRTR = %02x\n", TX.RTR);
	printf("canDLC = %02x\n", TX.DLC);

	for(i=0; i<len; i++)
		printf("TX.Buf[%d] = %02x\n", i, TX.Buf[i]);
}

uint8_t sendMsg(int fd)
{
	uint8_t res, res1;
	uint16_t uTimeOut = 0;

	do{
		res = mcp25625_getNextFreeTXBuf(fd, &tx_sidh);
		uTimeOut++;
	}
	while(res == MCP_ALLTXBUSY && (uTimeOut < TIMEOUTVALUE));
	printf("selected txbuf = %02x\n", tx_sidh);

	if(uTimeOut == TIMEOUTVALUE)
		return CAN_GETTXBFTIMEOUT;

	uTimeOut = 0;
	mcp25625_write_canMsg(fd, tx_sidh);

	mcp25625_modifyRegister(fd, tx_sidh -1, MCP_TXB_TXREQ_M, MCP_TXB_TXREQ_M);

	do{
		uTimeOut++;
		res1 = mcp25625_readRegister(fd, tx_sidh -1);
		res1 = res1 & 0x08;
	}
	while(res1 && (uTimeOut < TIMEOUTVALUE));

	if(uTimeOut == TIMEOUTVALUE)
		return CAN_SENDMSGTIMEOUT;

	return CAN_OK;
	
}

void mcp25625_write_canMsg(int fd, const uint8_t buffer_sidh_addr)
{
	uint8_t mcp_addr = buffer_sidh_addr;
	uint8_t txDLC = TX.DLC;
	unsigned char rts = 0x80;	
	unsigned char txbuf[14];
	memset(txbuf, 0, sizeof(txbuf));	

	mcp25625_setMsg(fd, &rts, txbuf, mcp_addr);

	do_msg(fd, &rts, 1);
}

void mcp25625_setMsg(int fd, unsigned char *rts, unsigned char * txbuf, const uint8_t mcp_addr)
{

	int i;
	uint8_t a1;
	uint16_t canid;
	unsigned char buf[4];
	uint8_t txDLC = TX.DLC;

	memset(buf, 0, sizeof(buf));
	
	canid = (uint16_t)(TX.EID & 0x0FFFF);

/* Extended ID */
	buf[MCP_EID0] = (uint8_t)(canid & 0xFF);
	buf[MCP_EID8] = (uint8_t)(canid >> 8);
	canid = (uint16_t)(TX.EID >> 16);
	buf[MCP_SIDL] = (uint8_t)(canid & 0x03);
	buf[MCP_SIDL] += (uint8_t)( (canid & 0x1C) << 3);
	buf[MCP_SIDL] |= 0x08;
	buf[MCP_SIDH] = (uint8_t)(canid >> 5);

	txbuf[0] = MCP_LD_TXBUF;
	txbuf[1] = buf[MCP_SIDH];
	txbuf[2] = buf[MCP_SIDL];
	txbuf[3] = buf[MCP_EID8];
	txbuf[4] = buf[MCP_EID0];


	
	if(TX.RTR == 1)
		txDLC |= MCP_RTR_MASK;

	txbuf[5] = txDLC;
	
	for(i=0; i<TX.DLC; i++)
		txbuf[i+6] = TX.Buf[i];

	mcp25625_loadTXBuffer(fd, rts, txbuf, mcp_addr);
}

void mcp25625_loadTXBuffer(int fd, unsigned char *rts, unsigned char * txbuf, const uint8_t buffer_addr)
{
	uint8_t n, i;
	unsigned char aPoint;

	switch(buffer_addr)
	{
		case MCP_TXB0SIDH:
		aPoint = 0x0;
		*rts |= 0x01;
		break;

		case MCP_TXB0D0:
		aPoint = 0x1;
		*rts |= 0x01;
		break;

		case MCP_TXB1SIDH:
		aPoint = 0x2;
		*rts |= 0x02;
		break;

		case MCP_TXB1D0:
		aPoint = 0x3;
		*rts |= 0x02;
		break;

		case MCP_TXB2SIDH:
		aPoint = 0x4;
		*rts |= 0x04;
		break;

		case MCP_TXB2D0:
		aPoint = 0x5;
		*rts |= 0x04;
		break;
	}

	txbuf[0] |= aPoint;
	do_msg(fd, txbuf, TX.DLC + 6);
}

/* Receive Msg */
uint8_t readMsgBuf(int fd, uint16_t *id, uint8_t *len, unsigned char *rxBuf, uint8_t RXxIF)
{
	uint8_t i;

	printf("readMsgBuf FUNC\n");

	if(readMsg(fd, RXxIF) == CAN_OK)
	{
		*id = RX.ID;
		*len = RX.DLC;

		if(RX.IDE)
		{
			RX.EID = 0x80000000;
		}

/*
		for(i=0;i<RX.DLC;i++)
			rxBuf[i] = RX.Buf[i];
*/		
		printf("Receive Success\n");
		return CAN_OK;
	}
	else
	{
		printf("Receive Fail\n");
		return CAN_FAIL;
	}
	
	printf("Receive Error\n");
	return CAN_ERROR;
}

uint8_t readMsg(int fd, uint8_t RXxIF)
{
	uint8_t  res;
	uint8_t nm;
	unsigned char stat;

	printf("readMsg FUNC\n");

	stat = RXxIF;	

	if( stat & MCP_STAT_RX0IF)
	{
		nm = 0;
		mcp25625_read_canMsg(fd, MCP_RXB0SIDH, nm);
		mcp25625_modifyRegister(fd, MCP_CANINTF, MCP_STAT_RX0IF, 0);
		res = CAN_OK;
	}	
	else if(stat & MCP_STAT_RX1IF)
	{
		nm = 1;
		mcp25625_read_canMsg(fd, MCP_RXB1SIDH, nm);
		mcp25625_modifyRegister(fd, MCP_CANINTF, MCP_STAT_RX1IF, 0);
		res = CAN_OK;
	}
	else
	{
		res = CAN_FAIL;
	}
	
	return res;
}

uint8_t mcp25625_readStatus(int fd)
{
	unsigned char buf[2];

	memset(buf, 0, sizeof(buf));

	buf[0] = MCP_READ_STATUS;

	do_msg(fd, buf, sizeof(buf));
	
	buf[0] = buf[0] & 0x03;
	return buf[0];	
}

void mcp25625_read_canMsg(int fd, const uint8_t buffer_sidh_addr, uint8_t nm)
{
	uint8_t mcp_addr, ctrl;
	//uint8_t ide;
	//uint16_t id;	
	
	printf("read_canMsg FUNC\n");

	mcp_addr = buffer_sidh_addr;
	
	mcp25625_read_buf(fd, mcp_addr, &RX.IDE, &RX.ID, nm);

	//myCanMsg.canID = id;
	//myCanMsg.canIDE = ide;
}

void mcp25625_read_buf(int fd, const uint8_t mcp_addr, uint8_t *ext, uint16_t *id, uint8_t nm)
{
	uint8_t rxbuf[13];
	uint8_t ctrl;
	uint8_t i;	
	uint8_t offset;
	printf("read_buf FUNC\n");

	*id = 0;
	*ext = 0;
	
//	 mcp25625_ReadRXBuffer(fd, rxbuf, MCP_READ_RX_BUFFER, nm);
	for(i=0;i<13;i++)
	{
		rxbuf[i] = mcp25625_readRegister(fd, mcp_addr + i);
		printf("RXBUF[%d] = %x\n",i,rxbuf[i]);
	}	
	


	
	*id = (rxbuf[MCP_SIDH] << 3) + (rxbuf[MCP_SIDL] >> 5);
	printf("Std ID : 0x%x\n",*id);

	
	if( (rxbuf[MCP_SIDL + offset] & MCP_RXB_EXIDE_M) == MCP_RXB_EXIDE_M)
	{
		*ext = 1;
		*id = (*id << 2) + (rxbuf[MCP_SIDL] & 0x03);
		*id = (*id << 8) + (rxbuf[MCP_EID8]);
		*id = (*id << 8) + (rxbuf[MCP_EID0]);
		printf("Ext ID : 0x%x\n",*id);
	}

	
	
	ctrl = mcp25625_readRegister(fd, mcp_addr-1);
	printf("RXBxCTRL : 0x%x\n",ctrl);

	
	RX.DLC = rxbuf[4];
	printf("DLC : 0x%x\n",RX.DLC);

//	myCanMsg.canDLC = mcp25625_readRegister(fd, mcp_addr+4);
//	printf("DLC : 0x%x\n",myCanMsg.canDLC);	

	for(i=0; i<RX.DLC; i++)
	{
		RX.Buf[i] = rxbuf[i + 5 ];
		printf("RXBUF DATA = 0x%x\n",RX.Buf[i]);
	}

	if(ctrl & MCP_RX_RTR)
		RX.RTR = 1;
	else
		RX.RTR = 0;

	printf("RTR : 0x%x\n",RX.RTR);
}

void mcp25625_ReadRXBuffer(int fd, unsigned char * rxbuf, const uint8_t buffer_addr, uint8_t nm)
{
	int i;
	printf("ReadRXBuffer Func\n");

	if(nm == 0)		
	{
		rxbuf[0] = buffer_addr & 0xF9;	
	}
	else if(nm == 1)
	{
		rxbuf[0] = buffer_addr & 0xF9;
		rxbuf[0] |= 0x04;
	}

	do_msg(fd, rxbuf, 13);
	
	for(i=0;i<13;i++)
	{
		printf("RXB[%d] = 0x%x\n",i,rxbuf[i]);
	}
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

static void do_msg(int fd, unsigned char * buf, int len)
{
	struct spi_ioc_transfer	xfer[2];
	unsigned char			*bp;
	int						status;

	memset(xfer, 0, sizeof xfer);

	xfer[0].tx_buf = (unsigned long)buf;
	xfer[0].len = len;

	xfer[1].rx_buf = (unsigned long) buf;
	xfer[1].len = len;

	status = ioctl(fd, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0) {

		printf("status : %d\n", status);
		perror("SPI_IOC_MESSAGE");
		return;
	}

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

	printf("read(%2d, %2d): %02x %02x,", len, status, buf[0], buf[1]);

	status -= 2;
	bp = buf + 2;

	while (status-- > 0)
		printf(" %02x", *bp++);
	printf("\n");
}

