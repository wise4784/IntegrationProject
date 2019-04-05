/*
 * mpu9250.h
 *
 *  Created on: 2019. 3. 26.
 *      Author: roro
 */

#ifndef INCLUDE_MPU9250_H_
#define INCLUDE_MPU9250_H_

#include "HL_sys_common.h"
#include "HL_system.h"
#include "HL_etpwm.h"
#include "HL_sci.h"
#include "HL_gio.h"
#include "HL_i2c.h"
#include "HL_rti.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#define UART sciREG1

#define AK8963_ADDRESS   0x0C
#define WHO_AM_I_AK8963  0x00
#define INFO             0x01
#define AK8963_ST1       0x02
#define AK8963_XOUT_L    0x03
#define AK8963_XOUT_H    0x04
#define AK8963_YOUT_L    0x05
#define AK8963_YOUT_H    0x06
#define AK8963_ZOUT_L    0x07
#define AK8963_ZOUT_H    0x08
#define AK8963_ST2       0x09
#define AK8963_CNTL      0x0A
#define AK8963_ASTC      0x0C
#define AK8963_I2CDIS    0x0F
#define AK8963_ASAX      0x10
#define AK8963_ASAY      0x11
#define AK8963_ASAZ      0x12

#define SELF_TEST_X_GYRO 0x00
#define SELF_TEST_Y_GYRO 0x01
#define SELF_TEST_Z_GYRO 0x02

/*#define X_FINE_GAIN      0x03
 #define Y_FINE_GAIN      0x04
 #define Z_FINE_GAIN      0x05
 #define XA_OFFSET_H      0x06
 #define XA_OFFSET_L_TC   0x07
 #define YA_OFFSET_H      0x08
 #define YA_OFFSET_L_TC   0x09
 #define ZA_OFFSET_H      0x0A
 #define ZA_OFFSET_L_TC   0x0B */

#define SELF_TEST_X_ACCEL 0x0D
#define SELF_TEST_Y_ACCEL 0x0E
#define SELF_TEST_Z_ACCEL 0x0F

#define SELF_TEST_A      0x10

#define XG_OFFSET_H      0x13
#define XG_OFFSET_L      0x14
#define YG_OFFSET_H      0x15
#define YG_OFFSET_L      0x16
#define ZG_OFFSET_H      0x17
#define ZG_OFFSET_L      0x18
#define SMPLRT_DIV       0x19
#define CONFIG           0x1A
#define GYRO_CONFIG      0x1B
#define ACCEL_CONFIG     0x1C
#define ACCEL_CONFIG2    0x1D
#define LP_ACCEL_ODR     0x1E
#define WOM_THR          0x1F

#define MOT_DUR          0x20
#define ZMOT_THR         0x21
#define ZRMOT_DUR        0x22

#define FIFO_EN          0x23
#define I2C_MST_CTRL     0x24
#define I2C_SLV0_ADDR    0x25
#define I2C_SLV0_REG     0x26
#define I2C_SLV0_CTRL    0x27
#define I2C_SLV1_ADDR    0x28
#define I2C_SLV1_REG     0x29
#define I2C_SLV1_CTRL    0x2A
#define I2C_SLV2_ADDR    0x2B
#define I2C_SLV2_REG     0x2C
#define I2C_SLV2_CTRL    0x2D
#define I2C_SLV3_ADDR    0x2E
#define I2C_SLV3_REG     0x2F
#define I2C_SLV3_CTRL    0x30
#define I2C_SLV4_ADDR    0x31
#define I2C_SLV4_REG     0x32
#define I2C_SLV4_DO      0x33
#define I2C_SLV4_CTRL    0x34
#define I2C_SLV4_DI      0x35
#define I2C_MST_STATUS   0x36
#define INT_PIN_CFG      0x37
#define INT_ENABLE       0x38
#define DMP_INT_STATUS   0x39
#define INT_STATUS       0x3A
#define ACCEL_XOUT_H     0x3B
#define ACCEL_XOUT_L     0x3C
#define ACCEL_YOUT_H     0x3D
#define ACCEL_YOUT_L     0x3E
#define ACCEL_ZOUT_H     0x3F
#define ACCEL_ZOUT_L     0x40
#define TEMP_OUT_H       0x41
#define TEMP_OUT_L       0x42
#define GYRO_XOUT_H      0x43
#define GYRO_XOUT_L      0x44
#define GYRO_YOUT_H      0x45
#define GYRO_YOUT_L      0x46
#define GYRO_ZOUT_H      0x47
#define GYRO_ZOUT_L      0x48
#define EXT_SENS_DATA_00 0x49
#define EXT_SENS_DATA_01 0x4A
#define EXT_SENS_DATA_02 0x4B
#define EXT_SENS_DATA_03 0x4C
#define EXT_SENS_DATA_04 0x4D
#define EXT_SENS_DATA_05 0x4E
#define EXT_SENS_DATA_06 0x4F
#define EXT_SENS_DATA_07 0x50
#define EXT_SENS_DATA_08 0x51
#define EXT_SENS_DATA_09 0x52
#define EXT_SENS_DATA_10 0x53
#define EXT_SENS_DATA_11 0x54
#define EXT_SENS_DATA_12 0x55
#define EXT_SENS_DATA_13 0x56
#define EXT_SENS_DATA_14 0x57
#define EXT_SENS_DATA_15 0x58
#define EXT_SENS_DATA_16 0x59
#define EXT_SENS_DATA_17 0x5A
#define EXT_SENS_DATA_18 0x5B
#define EXT_SENS_DATA_19 0x5C
#define EXT_SENS_DATA_20 0x5D
#define EXT_SENS_DATA_21 0x5E
#define EXT_SENS_DATA_22 0x5F
#define EXT_SENS_DATA_23 0x60
#define MOT_DETECT_STATUS 0x61
#define I2C_SLV0_DO      0x63
#define I2C_SLV1_DO      0x64
#define I2C_SLV2_DO      0x65
#define I2C_SLV3_DO      0x66
#define I2C_MST_DELAY_CTRL 0x67
#define SIGNAL_PATH_RESET  0x68
#define MOT_DETECT_CTRL  0x69
#define USER_CTRL        0x6A
#define PWR_MGMT_1       0x6B
#define PWR_MGMT_2       0x6C
#define DMP_BANK         0x6D
#define DMP_RW_PNT       0x6E
#define DMP_REG          0x6F
#define DMP_REG_1        0x70
#define DMP_REG_2        0x71
#define FIFO_COUNTH      0x72
#define FIFO_COUNTL      0x73
#define FIFO_R_W         0x74
#define WHO_AM_I_MPU9250 0x75
#define XA_OFFSET_H      0x77
#define XA_OFFSET_L      0x78
#define YA_OFFSET_H      0x7A
#define YA_OFFSET_L      0x7B
#define ZA_OFFSET_H      0x7D
#define ZA_OFFSET_L      0x7E

#define ADO 0
#if ADO
#define MPU9250_ADDRESS 0x69
#else
#define MPU9250_ADDRESS 0x68
#define AK8963_ADDRESS 0x0C
#endif

#define AHRS true
#define SerialDebug true

enum Ascale {
    AFS_2G = 0,
    AFS_4G,
    AFS_8G,
    AFS_16G
};

enum Gscale {
    GFS_250DPS = 0,
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

enum Mscale {
    MFS_14BITS = 0,
    MFS_16BITS
};

uint8 Gscale = GFS_500DPS;
uint8 Ascale = AFS_2G;
uint8 Mscale = MFS_16BITS;
uint8 Mmode = 0x02;

float aRes, gRes, mRes;

int16_t g_xyz[3];
long gyro_x_cal, gyro_y_cal, gyro_z_cal;
boolean set_gyro_angles;

int16_t a_xyz[3];
long acc_total_vector;
float angle_roll_acc, angle_pitch_acc;

float angle_pitch, angle_roll;
float angle_pitch_output, angle_roll_output;


void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 len);

uint8 readByte(uint8 devAddr, uint8 regAddr);
void readBytes(uint8 devAddr, uint8 regAddr, uint8 length, uint8 *data);
void writeByte(uint8 devAddr, uint8 regAddr, uint8 data);

void wait(uint32 delay);

void initMPU9250(void);
void initAK8963(float * destination);
void calibrateMPU9250(float * dest1, float * dest2);

void getAres(void);
void getMres(void);
void getGres(void);

void readAccelData(int16_t* destination);
void readGyroData(int16_t * destination);
void readMagData(int16_t * destination);
void get_roll_pitch(void);
void get_offset_value(void);

uint8 readByte(uint8 devAddr, uint8 regAddr)
{
    uint8 data;
    i2cSetSlaveAdd(i2cREG2, devAddr);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    i2cSetCount(i2cREG2, 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, regAddr);

    while(i2cIsBusBusy(i2cREG2) == true)
        ;
    while(i2cIsStopDetected(i2cREG2) == 0)
        ;

    i2cSetDirection(i2cREG2, I2C_RECEIVER);
    i2cSetCount(i2cREG2, 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStart(i2cREG2);

    i2cReceive(i2cREG2, 1, &data);
    i2cSetStop(i2cREG2);

    while(i2cIsBusBusy(i2cREG2) == true)
        ;
    while(i2cIsStopDetected(i2cREG2) == 0)
        ;
    i2cClearSCD(i2cREG2);

    return data;
}

void readBytes(uint8 devAddr, uint8 regAddr, uint8 length, uint8 *data)
{
    i2cSetSlaveAdd(i2cREG2, devAddr);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    i2cSetCount(i2cREG2, 1);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, regAddr);

    while(i2cIsBusBusy(i2cREG2) == true)
        ;
    while(i2cIsStopDetected(i2cREG2) == 0)
        ;

    i2cSetDirection(i2cREG2, I2C_RECEIVER);
    i2cSetCount(i2cREG2, length);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStart(i2cREG2);

    i2cReceive(i2cREG2, length, data);
    i2cSetStop(i2cREG2);

    while(i2cIsBusBusy(i2cREG2) == true)
        ;
    while(i2cIsStopDetected(i2cREG2) == 0)
        ;
    i2cClearSCD(i2cREG2);
}

void writeByte(uint8 devAddr, uint8 regAddr, uint8 data)
{
    i2cSetSlaveAdd(i2cREG2, devAddr);
    i2cSetDirection(i2cREG2, I2C_TRANSMITTER);
    i2cSetCount(i2cREG2, 2);
    i2cSetMode(i2cREG2, I2C_MASTER);
    i2cSetStop(i2cREG2);
    i2cSetStart(i2cREG2);
    i2cSendByte(i2cREG2, regAddr);
    i2cSend(i2cREG2, 1, &data); //

    while(i2cIsBusBusy(i2cREG2) == true)
        ;
    while(i2cIsStopDetected(i2cREG2) == 0)
        ;
    i2cClearSCD(i2cREG2);

    wait(100000);
}

void wait(uint32 delay)
{
    int i;

    for (i = 0; i < delay; i++)
        ;
}

void disp_set(char *str)
{
    char txt_buf[256] = {0};
    unsigned int buf_len;
    sprintf(txt_buf, str);
    buf_len = strlen(txt_buf);
    sciDisplayText(sciREG1, (uint8 *)txt_buf, buf_len);
    wait(100000);
}

void sciDisplayText(sciBASE_t *sci, uint8 *text, uint32 len)
{
    while(len--)
    {
        while((sci->FLR & 0x4) == 4)
            ;
        sciSendByte(sci, *text++);
    }
}

void sciReadText(sciBASE_t *sci, uint32 *text, uint32 len)
{
    while(len--)
    {
        while((sci->FLR & 0x4)==4)
            ;
        *text++ = sciReceiveByte(sci);

    }
}

void calibrateMPU9250(float * dest1, float * dest2)
{
    uint8_t data[12];
    uint16_t ii, packet_count, fifo_count;
    int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

    //내부 레지스터 리셋, 디폴트 세팅으로 복원
    writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x80);
    wait(200);

    //클럭 소스 설정 - Auto
    writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
    //X,Y,Z accelerometer, X,Y,Z gyro disable
    writeByte(MPU9250_ADDRESS, PWR_MGMT_2, 0x00);
    wait(200);

    //인터럽트 기능 모두 disable
    writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x00);
    //FIFO_enable 기능 모두 disable
    writeByte(MPU9250_ADDRESS, FIFO_EN, 0x00);
    //클럭 소스를 internal 20MHz oscillator
    writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);
    //I2C Master Ctrl 초기화
    writeByte(MPU9250_ADDRESS, I2C_MST_CTRL, 0x00);
    //User Control 초기화
    writeByte(MPU9250_ADDRESS, USER_CTRL, 0x00);
    //FIFO 모듈 리셋
    writeByte(MPU9250_ADDRESS, USER_CTRL, 0x0C);
    wait(200);

    //DLPF_CFG = 01
    //Gyroscope BW = 184Hz, Delay = 2.9ms, Fs = 1kHz
    //Temperature Sensor Bw = 188Hz, Delay = 1.9ms
    writeByte(MPU9250_ADDRESS, CONFIG, 0x01);
    //Sample Rate Divider
    //Sample rate = internal_sample_rate / (1 + SMPLRT_DIV)
    writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x00);
    //Gyroscope Configuration
    //Fchoice_b[1:0] = 00
    writeByte(MPU9250_ADDRESS, GYRO_CONFIG, 0x00);
    //ACCEL_FS_SEF[1:0] = +-2g(00)
    writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, 0x00);

    uint16_t  gyrosensitivity  = 131;
    uint16_t  accelsensitivity = 16384;

    //Enable FIFO opertaion mode
    writeByte(MPU9250_ADDRESS, USER_CTRL, 0x40);
    //GYRO_X,Y,ZOUT을 FIFO에 쓴다
    //ACCEL_X,Y,ZOUT을 FIFO에 쓴다
    //SLAVE에 해당하는 EXT_SENS_DATA를 FIFO에 쓴다
    writeByte(MPU9250_ADDRESS, FIFO_EN, 0x78);
    wait(200);

    //FIFO_EN의 모든 기능을 disable
    writeByte(MPU9250_ADDRESS, FIFO_EN, 0x00);
    //FIFO_COUNTH 8비트(하위 4비트만)를 data[0]
    //FIFO_COUNTL 8비트를 data[1]에 저장
    readBytes(MPU9250_ADDRESS, FIFO_COUNTH, 2, &data[0]);
    //FIFO에 쓰여진 바이트를 나타냄
    fifo_count = ((uint16_t)data[0] << 8) | data[1];
    //우리가 원하는 정보는 data로 아래에서 12바이트씩 읽어보오기 때문에 12로 나누어줌
    packet_count = fifo_count/12;

    for (ii = 0; ii < packet_count; ii++)
    {
        int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
        //FIFO_R_W에 쓰여진 데이터를 12번 읽어서 data에 순서대로 씀
        readBytes(MPU9250_ADDRESS, FIFO_R_W, 12, &data[0]);
         //data의 2바이트씩 읽어서 16비트(2바이트) accel_temp, gyro_temp에 대입
        accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]);
        accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]);
        accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]);
        gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]);
        gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]);
        gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

        //accel_temp, gyro_temp 데이터 모두 더해서 accel_bias, gyro_bias에 저장
        accel_bias[0] += (int32_t) accel_temp[0];
        accel_bias[1] += (int32_t) accel_temp[1];
        accel_bias[2] += (int32_t) accel_temp[2];
        gyro_bias[0]  += (int32_t) gyro_temp[0];
        gyro_bias[1]  += (int32_t) gyro_temp[1];
        gyro_bias[2]  += (int32_t) gyro_temp[2];
    }

    //packet_count로 나누어서 평균을 구함 -> accel_bias, gyro_bias 설정
    accel_bias[0] /= (int32_t) packet_count;
    accel_bias[1] /= (int32_t) packet_count;
    accel_bias[2] /= (int32_t) packet_count;
    gyro_bias[0]  /= (int32_t) packet_count;
    gyro_bias[1]  /= (int32_t) packet_count;
    gyro_bias[2]  /= (int32_t) packet_count;

    //accel_bias[2](z축) 값 보정
    if(accel_bias[2] > 0L)
    {
        accel_bias[2] -= (int32_t) accelsensitivity;
    }
    else
    {
        accel_bias[2] += (int32_t) accelsensitivity;
    }

    //OffsetLSB = X(or Y or Z)_OFFS_USR * 4 / 2^FS_SEL
    data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF;
    data[1] = (-gyro_bias[0]/4)       & 0xFF;
    data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
    data[3] = (-gyro_bias[1]/4)       & 0xFF;
    data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
    data[5] = (-gyro_bias[2]/4)       & 0xFF;

    //X,Y,ZG_OFFSET을 씀(Gyro offset설정)
    writeByte(MPU9250_ADDRESS, XG_OFFSET_H, data[0]);
    writeByte(MPU9250_ADDRESS, XG_OFFSET_L, data[1]);
    writeByte(MPU9250_ADDRESS, YG_OFFSET_H, data[2]);
    writeByte(MPU9250_ADDRESS, YG_OFFSET_L, data[3]);
    writeByte(MPU9250_ADDRESS, ZG_OFFSET_H, data[4]);
    writeByte(MPU9250_ADDRESS, ZG_OFFSET_L, data[5]);

    //Offset DPS(degree per second)
    //     = X(or Y or Z)_OFFS_USR * 4/ 2^FS_SEL /Gyro_Sensitivity
    //Offset DPS 설정
    dest1[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
    dest1[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
    dest1[2] = (float) gyro_bias[2]/(float) gyrosensitivity;


    int32_t accel_bias_reg[3] = {0, 0, 0};
    //X(or Y or Z)A_OFFSET_H에 저장된 데이터를 읽어서 accel_bias_reg에 저장
    //readBytes(MPU9250_ADDRESS, XA_OFFSET_H, 2, &data[0]);
    accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
    //readBytes(MPU9250_ADDRESS, YA_OFFSET_H, 2, &data[0]);
    accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
    //readBytes(MPU9250_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
    accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

    uint32_t mask = 1uL;
    uint8_t mask_bit[3] = {0, 0, 0};

    //mask_bit 설정
    //accel_bias_reg의 0번 비트가 1이면 mask_bit를 1로 설정
    for(ii = 0; ii < 3; ii++)
    {
        if((accel_bias_reg[ii] & mask))
            mask_bit[ii] = 0x01;
    }

    //?????
    accel_bias_reg[0] -= (accel_bias[0]/8);
    accel_bias_reg[1] -= (accel_bias[1]/8);
    accel_bias_reg[2] -= (accel_bias[2]/8);

    //???????
    data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
    data[1] = (accel_bias_reg[0])      & 0xFF;
    data[1] = data[1] | mask_bit[0];
    data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
    data[3] = (accel_bias_reg[1])      & 0xFF;
    data[3] = data[3] | mask_bit[1];
    data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
    data[5] = (accel_bias_reg[2])      & 0xFF;
    data[5] = data[5] | mask_bit[2];

/*
    //Accelerometer Offset Register 설정
    writeByte(MPU9250_ADDRESS, XA_OFFSET_H, (int8_t) (data2[0] >> 8));
    writeByte(MPU9250_ADDRESS, XA_OFFSET_L, (int8_t)data2[0]);
    writeByte(MPU9250_ADDRESS, YA_OFFSET_H, (int8_t) (data2[1] >> 8));
    writeByte(MPU9250_ADDRESS, YA_OFFSET_L, (int8_t)data2[1]);
    writeByte(MPU9250_ADDRESS, ZA_OFFSET_H, data2[2]);
    writeByte(MPU9250_ADDRESS, ZA_OFFSET_L, data2[2]);
*/

    //?????????
    dest2[0] = (float)accel_bias[0]/(float)accelsensitivity;
    dest2[1] = (float)accel_bias[1]/(float)accelsensitivity;
    dest2[2] = (float)accel_bias[2]/(float)accelsensitivity;


}
void getMres(void)
{
    switch (Mscale)
    {
    case MFS_14BITS:
        mRes = 10.0 * 4219.0/ 8190.0;
        break;
    case MFS_16BITS:
        mRes = 10.0 * 4219.0/ 32760.0;
        break;
    }
}

void getGres(void)
{
    switch (Gscale)
    {
    case GFS_250DPS:
        gRes = 250.0/32768.0;
        break;
    case GFS_500DPS:
        gRes = 500.0/32768.0;
        break;
    case GFS_1000DPS:
        gRes = 1000.0/32768.0;
        break;
    case GFS_2000DPS:
        gRes = 2000.0/32768.0;
        break;
    }
}
void getAres(void)
{
    switch (Ascale)
    {
    case AFS_2G:
        aRes = 2.0/32768.0;
        break;
    case AFS_4G:
        aRes = 4.0/32768.0;
        break;
    case AFS_8G:
        aRes = 8.0/32768.0;
        break;
    case AFS_16G:
        aRes = 16.0/32768.0;
        break;
    }
}

void initMPU9250(void)
{
      //MWR_MGMT 초기화
     writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x00);
     wait(1000);

      //클럭 소스를 가장 적절한 소스로 auto select
     writeByte(MPU9250_ADDRESS, PWR_MGMT_1, 0x01);
     wait(1000);

     //DLPF_CFG 설정(Digital Low Pass Filter)
     //Gyroscope BW = 41Hz, Delay = 5.9ms, Fs = 1kHz
     //Temperature Sensor Bw = 42Hz, Delay = 4.8ms
     writeByte(MPU9250_ADDRESS, CONFIG, 0x03);

     //Sample Rate Divider 설정
     // SAMPLE_RATE = internal_sample_rate / (1 + SMPLRT_DIV)
     writeByte(MPU9250_ADDRESS, SMPLRT_DIV, 0x04);

     //GYRO_CONFIG 값을 c에 저장
     //GYRO_CONFIG = c = 0x00
     uint8 c = readByte(MPU9250_ADDRESS, GYRO_CONFIG);

     //Gscale = GFS_500DPS = 1;
     //c = 0x00
     // 0000 0010 | 0001 1000 = 0001 1010
     // ~(0001 1010) = 1110 0101
     // c | 1110 0101 = 0000 0000
     // 0000 0000 | 0000 1000 = 0000 1000
     //GYRO_CONFIG = 0000 1000 -> +500dps 설정
     writeByte(MPU9250_ADDRESS, GYRO_CONFIG, (c & ~(0x02 | 0x18)) | Gscale << 3);

     //ACCEL_CONFIG = c = 0x00
     //ACCEL_CONFIG를 c에 저장
     c = readByte(MPU9250_ADDRESS, ACCEL_CONFIG);

     //Ascale = 0
     //0x18 = 0001 1000
     //~0x18 = 1110 0111
     // c & ~0x18 = 0000 0000
     // Ascale << 3 = 0000 0000
     writeByte(MPU9250_ADDRESS, ACCEL_CONFIG, (c & ~0x18) | Ascale << 3);

     // c = 0x00
     c = readByte(MPU9250_ADDRESS, ACCEL_CONFIG2);

     // 0x0F = 0000 1111
     // ~0x0F = 1111 0000
     // c & ~0x0F = 0000 0000
     // (c & ~0x0F) | 0x03 = 0000 0011
     //ACCEL_CONFIG2 = 0x03 저장
     // A_DLPF_CFG = 3, Output BW = 41Hz, Delay = 11.80ms, noise Density = 250ug/rtHz, rate = 1kHz
     writeByte(MPU9250_ADDRESS, ACCEL_CONFIG2, (c & ~0x0F) | 0x03 );
     // 0x22 = 0010 0010
     // LATCH_INT_EN = 1, BYPASS_EN = 1
     writeByte(MPU9250_ADDRESS, INT_PIN_CFG, 0x22);
     // Raw Sensor Data Ready 인터럽트를 활성화
     writeByte(MPU9250_ADDRESS, INT_ENABLE, 0x01);
     wait(1000);
}

void initAK8963(float * destination)
{
    uint8_t rawData[3];
    //AK8963_ADDRESS = 0x0C, AK8963_CNTL = 0x0A
    // AK8963_CNTL에 0000을 쓰명 power-down mode
    writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00);
    wait(1000);
    // AK8963_CNTL에 0x0F를 쓰면 Fuse ROM access mode
    writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x0F);
    wait(1000);
    // AK8963_ASAX(or Y or Z) Sensitivity Adjustment values
    // Magnetic sensor X-axis sensitivity adjustment values
    // rawData에 3바이트를 씀, ASAX/ASAY/ASAZ
    readBytes(AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]);
    // sensitivity adjustment
    // Hadj = H x ((ASA - 128) x 0.5 / 128 + 1)
    destination[0] =  ((float)(rawData[0] - 128))/256.0 + 1.0;
    destination[1] =  ((float)(rawData[1] - 128))/256.0 + 1.0;
    destination[2] =  ((float)(rawData[2] - 128))/256.0 + 1.0;
    // 다시 power-down mode로
    writeByte(AK8963_ADDRESS, AK8963_CNTL, 0x00);
    wait(100);

    // Mscale = MFS_16BITS = 1
    // Mscale << 4 = 0001 0000
    // Mmode = 0x02
    // Mscale << 4 | Mmode = 0x10 | 0x02 = 0001 0010
    // CNTL1에서 BIT = 1이면 16-bit output
    //          MODE = 0010 이면 continuous measurement mode 1
    writeByte(AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode);
    wait(1000);
}

void get_offset_value(void)
{
    int cnt;
    for (cnt = 0; cnt < 1000; cnt++)
    {
        //1000번 동안 g_xyz에 Gyro offset저장
        readGyroData(g_xyz);
        //gyro_x,y,z_cal에 대입
        gyro_x_cal += g_xyz[0];
        gyro_y_cal += g_xyz[1];
        gyro_z_cal += g_xyz[2];
        wait(100);
    }

    //1000번의 평균값을 구함
    gyro_x_cal /= 1000;
    gyro_y_cal /= 1000;
    gyro_z_cal /= 1000;
}

void get_roll_pitch(void)
{
    readAccelData(a_xyz);
    readGyroData(g_xyz);

    g_xyz[0] -= gyro_x_cal;
    g_xyz[1] -= gyro_y_cal;
    g_xyz[2] -= gyro_z_cal;

    angle_pitch += g_xyz[0] * 0.0000611;
    angle_roll += g_xyz[1] * 0.0000611;

    angle_pitch += angle_roll * sin( ((float)g_xyz[2]) * 0.000001066);
    angle_roll -= angle_pitch * sin( ((float)g_xyz[2]) * 0.000001066);

    acc_total_vector = sqrt( pow(a_xyz[0],2)+pow(a_xyz[1],2)+pow(a_xyz[2],2) );

    angle_pitch_acc = asin( (float)a_xyz[1]/acc_total_vector) * 57.296;
    angle_roll_acc = asin( (float)a_xyz[0]/acc_total_vector) * -57.296;

//    angle_pitch_acc -= 0.0;
//    angle_roll_acc -= 0.0;

//    if(set_gyro_angles){
//      angle_pitch = angle_pitch * 0.9996 + angle_pitch_acc * 0.0004;
//      angle_roll = angle_roll * 0.9996 + angle_roll_acc * 0.0004;
//    }
//    else{
//      angle_pitch = angle_pitch_acc;
//      angle_roll = angle_roll_acc;
//      set_gyro_angles = true;
//    }
//
//    angle_pitch_output = angle_pitch_output * 0.9 + angle_pitch * 0.1;
//    angle_roll_output = angle_roll_output * 0.9 + angle_roll * 0.1;
}

void readAccelData(int16_t* destination)
{
    uint8_t rawData[6];
    readBytes(MPU9250_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);
    destination[0] = ((int16_t)rawData[0] << 8) | rawData[1];
    destination[1] = ((int16_t)rawData[2] << 8) | rawData[3];
    destination[2] = ((int16_t)rawData[4] << 8) | rawData[5];
}

void readGyroData(int16_t * destination)
{
     uint8_t rawData[6];
     // GYRO_XOUT_H 부터 6바이트를 읽어서 1바이트씩 rawData에 저
     readBytes(MPU9250_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);
     // 2바이트씩 붙여서 destination에 저장
     destination[0] = ((int16_t)rawData[0] << 8) | rawData[1];
     destination[1] = ((int16_t)rawData[2] << 8) | rawData[3];
     destination[2] = ((int16_t)rawData[4] << 8) | rawData[5];
}

void readMagData(int16_t * destination)
{
    uint8_t rawData[7];
    if(readByte(AK8963_ADDRESS, AK8963_ST1) & 0x01)
    {
        readBytes(AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]);
        uint8_t c = rawData[6];

        if(!(c & 0x08))
        {
            destination[0] = ((int16_t)rawData[1] << 8) | rawData[0];
            destination[1] = ((int16_t)rawData[3] << 8) | rawData[2];
            destination[2] = ((int16_t)rawData[5] << 8) | rawData[4];
        }
    }
}

#endif /* INCLUDE_MPU9250_H_ */
