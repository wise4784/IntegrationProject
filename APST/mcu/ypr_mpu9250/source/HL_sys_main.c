/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
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


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include "mpu9250.h"
//#include "Matrix.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
int i;

float real_elev,real_rudd,real_ail,throttle;
float quad_start;

float roll_target_angle = 0.0;
float roll_angle_in;
float roll_rate_in;
float roll_stabilize_kp = 3.5;  // angle kp
float roll_stabilize_ki = 0;  // angle ki
float roll_rate_kp = 0.004;  // angle rate kp setting
float roll_rate_ki = 0.0035;  // angle rate ki setting
float roll_rate_kd = 0.0006;  // angle rate kd setting
float roll_prev_rate_error = 0;
float roll_stabilize_iterm;
float roll_rate_iterm;
float roll_output;

float pitch_target_angle = 0.0;
float pitch_angle_in;
float pitch_rate_in;
float pitch_stabilize_kp = 3.5;
float pitch_stabilize_ki = 0;
float pitch_rate_kp = 0.004;
float pitch_rate_ki = 0.0035;
float pitch_rate_kd = 0.0006;
float pitch_prev_rate_error = 0;
float pitch_stabilize_iterm = 0;
float pitch_rate_iterm = 0;
float pitch_output = 0;

float yaw_target_angle = 0.0;
float yaw_angle_in;
float yaw_rate_in;
float yaw_stabilize_kp = 3.5;
float yaw_stabilize_ki = 0;
float yaw_rate_kp = 0.033;
float yaw_rate_ki = 0.0035;
float yaw_rate_kd = 0;
float yaw_prev_rate_error = 0;
float yaw_stabilize_iterm = 0;
float yaw_rate_iterm = 0;
float yaw_output = 0;

void initYPR(void)
{
    /*for mpu9250*/
    get_YPR();

    init_roll = roll;
    init_pitch = pitch;
    init_yaw = yaw;
    roll_target_angle = init_roll;
    pitch_target_angle = init_pitch;
    yaw_target_angle = init_yaw;

    /*for mpu6050*/
//    get_mpu6050_RPY();
//    roll_target_angle = real_roll;
//    pitch_target_angle = real_pitch;
//    yaw_target_angle = real_yaw;
}
/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    char txt_buf[256] = { 0 };
    unsigned int buf_len;

    sciInit();
    disp_set("SCI Configuration Success!!\n\r\0");

    i2cInit();
    wait(10000000);
    disp_set("I2C Init Success!!\n\r\0");

    uint8 c = readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
    sprintf(txt_buf, "I AM = %x\n\r\0", c);
    buf_len = strlen(txt_buf);
    sciDisplayText(sciREG1, (uint8 *) txt_buf, buf_len);

    if (c == 0x71)
    {
        calibrateMPU9250(gyroBias, accelBias);
        disp_set("MPU9250 calibration Success!!!!!!\n\r\0");

        initMPU9250();
        disp_set("MPU9250 Init Success!!!!!!\n\r\0");

        initAK8963(magCalibration);
        disp_set("MPU9250 AK8963 Init Success!!!!!!\n\r\0");

        get_offset_value();
        disp_set("gyro_offset_setting Success!!\n\r\0");

        initYPR();

        disp_set("Init YPR Success!!\n\r\0");
        wait(1000000);
    }
    else
    {
        disp_set("MPU9250 doesn`t work!!!");
        while (1)
            ;
    }

    for (;;)
    {
//         get_mpu6050_RPY();
//         get_mpu6050_gyro();

        if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
        {
            readAccelData(accelCount);
            getAres();

            ax = (float) accelCount[0] * aRes; // - accelBias[0];  // get actual g value, this depends on scale being set
            ay = (float) accelCount[1] * aRes; // - accelBias[1];
            az = (float) accelCount[2] * aRes; // - accelBias[2];

            readGyroData(gyroCount);  // Read the x/y/z adc values
            getGres();

            // Calculate the gyro value into actual degrees per second
            gx = (float) gyroCount[0] * gRes; // get actual gyro value, this depends on scale being set
            gy = (float) gyroCount[1] * gRes;
            gz = (float) gyroCount[2] * gRes;

            get_YPR();

            sprintf(txt_buf, "roll = %f \t pitch = %f \t yaw = %f \n\r\0",
                    angle_pitch_acc, angle_roll_acc, yaw * R2D);

            buf_len = strlen(txt_buf);
            sciDisplayText(sciREG1, (uint8 *) txt_buf, buf_len);
            wait(3000000);
        }
    }
/* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/* USER CODE END */
