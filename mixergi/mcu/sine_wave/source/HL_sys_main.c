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
#include "HL_sys_core.h"
#include "HL_gio.h"
#include "Sine_Wave.h"
#include "HL_het.h"
#include "HL_htu.h"
#include "HL_system.h"

/* USER CODE BEGIN (1) */
#define SINE_FREQ_DIVIDER   16
#define LRPFC               7
#define NHET1_PIN_PWM       PIN_HET_9
#define CNT_MAX_PERIOD      SINE_FREQ_DIVIDER
#define SAMPLE_SIZE         128
#define BUFFERSIZE          128      /* size of the single or double buffer */
#define INT                 0               /* 0 = interrupt disabled; 1 = interrupt enabled */
#define DOUBLEBUFFER        0      /* 0 = single buffer; 1 = double buffer */



#define SAMPLE_SIZE         128

#if SAMPLE_SIZE == 128
float sine_table_percent[SAMPLE_SIZE] = {0.5,0.52452141,0.548983805,0.573328313,0.597496346,
        0.62142974,0.645070896,0.668362917,0.691249749,0.71367631,0.735588627,0.756933966,
        0.777660956,0.797719715,0.817061967,0.835641164,0.853412591,0.870333478,0.886363105,
        0.901462892,0.9155965,0.928729914,0.940831527,0.951872215,0.961825406,0.970667147,
        0.978376158,0.984933888,0.990324553,0.994535181,0.997555637,0.999378653,0.999999841,
        0.999417707,0.997633651,0.994651967,0.990479831,0.985127283,0.978607206,0.970935291,
        0.962130001,0.952212527,0.941206738,0.929139121,0.916038718,0.901937056,0.886868075,
        0.870868039,0.853975454,0.836230976,0.81767731,0.798359106,0.778322858,0.757616784,
        0.736290719,0.714395985,0.691985276,0.669112527,0.645832783,0.622202072,0.598277264,
        0.574115936,0.549776238,0.525316747,0.500796326,0.47627399,0.451808753,0.427459496,
        0.403284818,0.379342899,0.355691359,0.332387119,0.309486264,0.287043908,0.265114062,
        0.243749504,0.223001649,0.202920432,0.18355418,0.164949501,0.14715117,0.130202021,
        0.114142845,0.099012291,0.084846772,0.07168038,0.059544802,0.048469244,0.03848036,
        0.029602191,0.021856103,0.015260738,0.009831969,0.00558286,0.002523639,0.000661668,
        0.000001426,0.000544506,0.002289597,0.005232501,0.009366135,0.014680552,0.02116296,
        0.02879776,0.037566577,0.047448308,0.05841917,0.070452761,0.08352012,0.097589799,
        0.112627937,0.128598342,0.14546258,0.163180064,0.181708154,0.20100226,0.221015947,
        0.241701051,0.263007789,0.284884883,0.307279683,0.330138292,0.353405699,0.377025906,
        0.400942069,0.425096628,0.449431454,0.47388798
};
#else
uint32 sine_table[SAMPLE_SIZE] = {0.5,0.548983805,0.597496346,0.645070896,0.691249749,0.735588627,
        0.777660956,0.817061967,0.853412591,0.886363105,0.9155965,0.940831527,0.961825406,0.978376158,
        0.990324553,0.997555637,0.999999841,0.997633651,0.990479831,0.978607206,0.962130001,
        0.941206738,0.916038718,0.886868075,0.853975454,0.81767731,0.778322858,0.736290719,
        0.691985276,0.645832783,0.598277264,0.549776238,0.500796326,0.451808753,0.403284818,
        0.355691359,0.309486264,0.265114062,0.223001649,0.18355418,0.14715117,0.114142845,
        0.084846772,0.059544802,0.03848036,0.021856103,0.009831969,0.002523639,0.000001426,
        0.002289597,0.009366135,0.02116296,0.037566577,0.05841917,0.08352012,0.112627937,
        0.14546258,0.181708154,0.221015947,0.263007789,0.307279683,0.353405699,0.400942069,
        0.449431454
 };
#endif

uint32 sine_table[SAMPLE_SIZE] = {0};
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
void calculate_ecmp_compare();
void configNHET1();
void htuInit();
/* USER CODE END */

//what does it mean?!
int main(void)
{
/* USER CODE BEGIN (3) */
    htuInit();
    hetInit();
    configNHET1();

    while(1);

/* USER CODE END */

}


/* USER CODE BEGIN (4) */
void configNHET1()
{
    hetREG1->PFR = LRPFC << 8;

    calculate_ecmp_compare();

    hetREG1->REQENS = 1;

    hetREG1->DIR = (1 << NHET1_PIN_PWM) ;

    hetRAM1->Instruction[pHET_L00_0].Control = (uint32)(CNT_MAX_PERIOD - 1) |
        (hetRAM1->Instruction[pHET_L00_0].Control & 0xFFFD0000);

    hetRAM1->Instruction[pHET_L01_0].Control =
                (hetRAM1->Instruction[pHET_L01_0].Control & 0xFFFFE0FF) |
                (NHET1_PIN_PWM << 8);
}

void calculate_ecmp_compare()
{
    int i = 0;
    for(i=0; i<=SAMPLE_SIZE; i++)
    {
        sine_table[i] = (sine_table_percent[i] * (CNT_MAX_PERIOD << 7));
    }
}

void htuInit()
{
    htuDCP1 ->ITCOUNT = 0x00010000 + SAMPLE_SIZE;
    htuDCP1 ->IHADDRCT = (htuDCP1 ->IHADDRCT & 0x0) |
                          0x1 << 23 |   // DIR
                          0x0 << 22 |   // SIZE
                          0x0 << 21 |   // ADDMH
                          0x0 << 20 |   // ADDFM
                          0x1 << 18 |   // TMBA
                          0x0 << 16 |   // TMBB
                          0x28 << 0;    // IHADDR

    htuDCP1 ->IFADDRA = (unsigned int)sine_table;

    htuREG1 ->CPENA = 0x00000001;
    htuREG1 ->GC = 0x00010000;
}
/* USER CODE END */
