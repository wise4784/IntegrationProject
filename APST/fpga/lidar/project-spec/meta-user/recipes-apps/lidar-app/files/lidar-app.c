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

unsigned get_status(void);
void lidar_read(unsigned char, unsigned, unsigned char *);
void lidar_write(unsigned char, unsigned char);
void measurement(unsigned char, unsigned char, unsigned char *);
void display(unsigned char, unsigned char *);

int fd = 0;

int main(int argc, char **argv)
{
    unsigned char receives[8] = {AR_VELOCITY, 0, 0,
	AR_PEAK_CORR, AR_NOISE_PEAK, AR_SIGNAL_STRENGTH, 
	AR_FULL_DELAY_HIGH, AR_FULL_DELAY_LOW};
    unsigned char i, options;
    char *fname = NULL;

    if(argc < 2)
	printf("%s\n", USAGE);
    else
	fname = I2C_DEV;

    options = atoi(argv[1]);

    if((fd = open(fname, O_RDWR)) < 0)
    {
	perror("Open error\n");
	return -1;
    }
    if(ioctl(fd, I2C_SLAVE, LIDAR_ADDR) < 0)
    {
	perror("Slave Addr Connect error");
	return -1;
    }

    lidar_write(SIG_COUNT_VAL, 0x80);
    lidar_write(ACQ_CONFIG_REG, 0x08);
    lidar_write(THRESHOLD_BYPASS, 0x00);

    for(;;)
    {
	measurement(CORRECTION, options, receives);

	for(i = 0; i < 99; i++)
	    measurement(NO_CORRECTION, options, receives);
    }

    close(fd);

    return 0;
}

unsigned get_status(void)
{
    unsigned char buf[1] = {STATUS};

    if(write(fd, buf, 1) != 1)
    {
	perror("Write Reg Error");
	return -1;
    }
    if(read(fd, buf, 1) != 1)
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
	    return;
	}
    }

    if(!busy_flag)
    {
	if(write(fd, buf, 1) != 1)
	{
	    perror("Write Reg Error");
	    return -1;
	}

	if(read(fd, receives, read_size) != read_size)
	{
	    perror("Read Reg Error");
	    return -1;
	}
    }
}

void lidar_write(unsigned char reg, unsigned char val)
{
    unsigned char buf[2] = {reg, val};

    if(write(fd, buf, 2) != 2)
    {
	perror("Write Reg Error");
	return -1;
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
	    printf("%s\t\t\t = %d\n", strings[4], buf[AR_FULL_DELAY_HIGH]);
	    break;
    }
    printf("\n");
}


