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

#define IN			0
#define OUT			1

#define GPIO_MAP_SIZE		0x10000
#define GPIO_DATA_OFFSET	0x00
#define GPIO_TRI_OFFSET		0x04


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

#define KEY_PMOD1	1234
#define KEY_PMOD2	1235
#define KEY_LIDAR	1236
#define KEY_INPUT	1237
#define KEY_VALUE1	1238
#define KEY_VALUE2	1239


void usage(void);

unsigned get_status(void);
void lidar_read(unsigned char, unsigned, unsigned char *);
void lidar_write(unsigned char, unsigned char);
void measurement(unsigned char, unsigned char, unsigned char *);
void display(unsigned char, unsigned char *);


/* LIDAR DATA */
int fd_lidar;
unsigned char receives[8] = {AR_VELOCITY, 0, 0,AR_PEAK_CORR, AR_NOISE_PEAK, AR_SIGNAL_STRENGTH, AR_FULL_DELAY_HIGH, AR_FULL_DELAY_LOW};
unsigned char options = 1;
char *fname;	
int *Lidar_Value;

int main(void)
{
/* TEST  */
	int sleepCount=0;
	int lidarCount=0;
	int inputCount=0;

/* FORK DATA*/
	int N = 3;
	pid_t pid[N];
	int i;

/*  INPUT DATA */
	int fd_input;
	int direction = IN;
	char *uiod;
	int value_input = 0;
	void *ptr;

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

/********************  pid[0] : LIDAR PROCESS  ************************************************/
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


		fname = "/dev/i2c-0";	
		fd_lidar = open(fname, O_RDWR);
	
		if(fd_lidar < 1)
		{
			printf("Lidar Failed\n");
//			return -1;
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
			lidarCount++;
			if(*flag_PmodToLidar)
			{
				measurement(CORRECTION, options, receives);
				*LidarValue = *Lidar_Value;
				for(i=0; i<99; i++)
				{
					measurement(NO_CORRECTION, options, receives);
				}
				printf("****************LIDAR VALUE : %d\n\n",*LidarValue);
				*flag_PmodToLidar = 0;
				*flag_LidarToPmod = 1;
			}
			if(lidarCount==1000)
			{
				printf("============LIDAR TEST============\n\n");
				lidarCount=0;
			}
		}

		close(fd_lidar);	
	}

/********************  pid[1] : INPUT PROCESS  ***********************************************/	
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


		uiod = "/dev/uio0";
		fd_input = open(uiod,O_RDWR);
		
		if(fd_input < 1)
		{
			printf("Invalid UIO device file : %s.\n",uiod);
			usage();
			return -1;
		}			
		
		ptr = mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED,fd_input,0);
		*((unsigned *)(ptr + GPIO_TRI_OFFSET)) = 1023;
		*InputValue = 0;	
		while(1)
		{
			inputCount++;
			if(*flag_PmodToInput)
			{
				*InputValue = *((unsigned *)(ptr + GPIO_DATA_OFFSET));
				//*InputValue = value_input;
				printf("***************INPUT Value : %08x\n\n",*InputValue);
				*flag_InputToPmod = 1;
				*flag_PmodToInput = 0;
			}
			if(inputCount==1000)
			{
				printf("=============INPUT TEST=========\n\n");
				inputCount=0;
			}
		}
		munmap(ptr,GPIO_MAP_SIZE);
		close(fd_input);
	}

/*******************  pid[2] : SPI PROCESS  **************************************************/
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

		while(1)
		{
			usleep(100000);
			*flag_PmodToInput = 1;
			sleepCount++;
			if(sleepCount == 5)
			{
				sleepCount = 0;
				*flag_PmodToLidar = 1;
			}

			if(*flag_LidarToPmod)
			{
				printf("LIDAR VALUE : %d\n",*LidarValue);
				*flag_LidarToPmod = 0;
				*LidarValue = 0;
			}
			if(*flag_InputToPmod)
			{
				printf("INPUT VALUE : %08x\n",*InputValue);
				*flag_LidarToPmod = 0;
				*InputValue = 0;
			}
		}

	}

/******************  PARENT PROCESS  *******************************************************/


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
    unsigned char i;
    char *strings[5] = {"Velocity", NULL, NULL, NULL, "Distance"};
    buf[AR_FULL_DELAY_HIGH] = buf[AR_FULL_DELAY_HIGH] << 8 | buf[AR_FULL_DELAY_LOW];
    switch(opt)
    {
	case DISTANCE_ONLY:
		Lidar_Value = (int *)buf;
	   // printf("%s\t\t\t = %d\n", strings[4], buf[AR_FULL_DELAY_HIGH]);
	    break;
    }
    printf("\n");
}


