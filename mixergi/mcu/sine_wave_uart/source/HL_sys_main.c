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
#include "HL_het.h"
#include "HL_sci.h"
#include "HL_sys_core.h"
#include "HL_gio.h"
#include "HL_htu.h"
#include "HL_system.h"
#include "sine_wave_uart.h"


void calculate_ecmp_compare();
void configNHET1();
void htuInit();
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function t    o implement the application.
*/
/* This example uses the below equations to calculate the SINE wave frequency (F):
 *
 * (1)  F = 1 / (PWM_PERIOD * samples) or F = PWM_FREQ / samples
 * (2)  PWM_PERIOD is the period of the PWM Base Frequency and
 *      PWM_PERIOD = (SINE_FREQ_DIVIDER  * LRP) where SINE_FREQ_DIVIDER >= 1
 * (3)  LRP = hr * lr. hr is fixed to 1 in this program. The supported lr
 *      in this program are 16, 32, 64, and 128. lr smaller than 16 will not
 *      give enough time slots for the N2HET to execute and also too short for
 *      the HTU to complete the transfer.
 * (4)  samples = the number of samples to digitize the sine wave
 *
* The fastest SINE frequency this example can support for SINE_FREQ_DIVIDER = 1 and
* samples = 128 would be:
 * F = 1 / ((SINE_FREQ_DIVIDER  * LRP) * Samples) or
 * F = 1 / ((1  * LRP) * 128)
 * Suppose HCLK=160MHz, VCLK2=80MHz, 1 LRP = 128 VCLK2 :
 * 1 LRP = 128 * 12.5ns = 1.6uS and F = 1 / (1.6uS * 128) ~= 4.88KHz
 *
 * With 1 LRP = 16 VCLK2 it is possible to generate a sine wave frequency at:
 * F = 1 / (16 * 12.5ns * 128) ~=39KHz.
 *
 * SINE_FREQ_DIVIDER is used to divide down the SINE frequency.
 * Setting SINE_FREQ_DIVIDER to 2 will generate 2.44KHz sine wave.
 * SINE_FREQ_DIVIDER can be any integer value larger or equal to 1
 */
#define SINE_FREQ_DIVIDER 16

/* allowable LR Prescaler factors are 16, 32, 64 and 128. Anything less
 * than 32 will not have enough time slots for the N2HET program to
 * run.

 * LRPFC can be either 5, 6 or 7.
 * 7 -> one lr = 128 VCLK2
 * 6 -> one lr = 64 VCLK2
 * 5 -> one lr = 32 VCLK2
 * 4 -> one lr = 16 VCLK2
 */

#define LRPFC   7
#define NHET1_PIN_PWM   PIN_HET_2

/****************************************************************************/
/* The PWM Period to be loaded to NHET1 CNT instruction. The minimum
 * PWM base frequency of the PWM is 1 * LRP */
#define CNT_MAX_PERIOD SINE_FREQ_DIVIDER

/* Number of sample points to digitize the SINE wave */
#define SAMPLE_SIZE 128

#if SAMPLE_SIZE == 128
/* sine_table_percent array contains the duty cycle of each sample point on the
 * sine wave. The percent values will then be converted to actual compare values
 * during run-time. The reason that the actual compare values are not hardcoded
 * is because this program is designed to support programmable sine frequency.
 * When the sine frequency is changed using SINE_FREQ_DIVIDER then the CNT_MAX_PERIOD
 * will need to be adjusted. The compare values need to be adjusted as well. Keeping
 * only the duty cycle percent values in the lookup table allows us to simply
 * calculate the final compare values by multiplying the percent with the CNT_MAX_PERIOD.
 *
 */
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
uint32 sine_table_percent[SAMPLE_SIZE] = {0.5,0.548983805,0.597496346,0.645070896,0.691249749,0.735588627,
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

    /*
     * There was the header file "main.h" in application zip file; spna217.zip
     *
     * I will include some contexts in this main file.
     * Hmm... no need these three macro values.
     */

//#define BUFFERSIZE      128 /* size of the single or double buffer */
//#define INT             0   /* 0 = interrupt disabled; 1 = interrupt enabled */
//#define DOUBLEBUFFER    0   /* 0 = single buffer; 1 = double buffer */

// Check sine_table[SAMPLE_SIZE] on Putty display
#define UART   sciREG1
void sci_display(sciBASE_t *sci, uint8 *text, uint32 len);
void wait(uint32 time);
/* sciREG1
 *
 */


/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */


    htuInit();
    hetInit();
    configNHET1();

    while(1)
    {
        ;
    }


}


/* USER CODE BEGIN (4) */

/* htuInit setups the HTU to transfer new compare value stored in
* the sine_table to the NHET ECMP instruction at each completion of
* one PWM period. The transfer is done in a circular fashion. */
void htuInit(void){
/*  DCP0 CPx element count = 1, frame count = SAMPLE_SIZE */
    htuDCP1 -> ITCOUNT = 0x00010000 + SAMPLE_SIZE;
    // 128 = 16 * 8 => 0x

/*  DCP0 CPx DIR = main memory to NHET
**  SIZE = 32-bit
**  ADDMH = 16 bytes
**  ADDFM = post-increment main memory
**  TMBA = circular buffer A
**  TMBB = one shot buffer B (buffer B not used)
**  IHADDR = 0x28 MOV32 data field
*/
    htuDCP1-> IHADDRCT = (htuDCP1-> IHADDRCT & 0x0) |
                                0x1 << 23 | // DIR
                                0x0 << 22 | // SIZE
                                0x0 << 21 | // ADDMH
                                0x0 << 20 | // ADDFM
                                0x1 << 18 | // TMBA
                                0x0 << 16 | // TMBB
                                0x28 << 0; // IHADDR
/* DCP0 CPA start address of source buffer */
    htuDCP1 -> IFADDRA = (unsigned int)sine_table;
/* enable DCP0 CPA */
    htuREG1 ->CPENA = 0x00000001;

/* enable HTU */
#if 1
    htuREG1 ->GC = 0x00010000;
#endif

/* Modify register*/
#if 0
    htuREG1 ->GC = 0x01010100;
#endif
    /* 24bit of GC is VBUSHOLD. If the 24bit is 1, VBUSHOLD bit holds the bus used to
     * transfer Data between HTU and N2HET module.
     * When the BUS_BUSY bit(See 20.3.1 Global Configuration Registers in DMA Module)
     * When the BUS_BUSY bit is 0 then the bus is no longer busy. While the bus is held,
     * requests will still be accepted. They will be acted upon when the VBUSHOLD is 0.
     * Request lost conditions will be detected and interrupts generated if they are enabled.
     */

    /* The Recommend order of operation is:
     * - Set the software bit(reset bit). This also clears HTUEN.
     * - Wait for the HTURES bit to clear.
     * - Configure the HTU registers and packets.
     * - Set the HTUEN bit to begin operation(16bit of GC is 1).
     */
}


/* configureNHET1 configures the selected NHET1 pin to output PWM. It
* also loads the specified PWM period into the NHET1 RAM */
void configNHET1()
{
/* configure the LRP prescaler, the hr is always 1 and lr can be
* either 16, 32, 64 or 128 */
    hetREG1->PFR = LRPFC << 8;
    /* HRPFC(High-Resolution Pre-scale Factor Code) dertermines
     * the high-resolution prescale value rate(hr). It is on 5-0 bit.
     *
     *  0: /1
     *  1: /2
     *  2: /4
     *  ...
     *  /3D: /62
     *  /3E: /63
     *  /3F: /64
     */

/* calculate_ecmp_compare() will calculate the actual compare values
* as well as the high resolution delay values for each sample point
* of the same wave to be loaded into the NHET1 ECMP pin instructions */

    calculate_ecmp_compare();

/* Enable DMA request on channel 0 of HTU. In the CNT instruction, the
* DMA request is asserted to channel 0 of the HTU module */
    hetREG1->REQENS = 1 ;
    /* Request Enable Set Register
     * 7-0 bit: REQENA7 to REQENA0
     * 0: disabled  1: enabled
     */

    /* Let's try to Request Destination Select!
    * N2HET request line 0 is assigned to DMA(TDBS bit 0 is ZERO)
    */
    //hetREG1->REQDS = 1 << 16;
/* Set the selected pin to output. */
    hetREG1->DIR = (1 << NHET1_PIN_PWM);
/* Load the PWM period based on the defined macro to the CNT instruction */
    hetRAM1->Instruction[pHET_L00_0].Control = (uint32)(CNT_MAX_PERIOD - 1 ) |
            (hetRAM1->Instruction[pHET_L00_0].Control & 0xFFFD0000);
/* Configure the pin number to output the PWM */
    hetRAM1->Instruction[pHET_L01_0].Control =
            (hetRAM1->Instruction[pHET_L01_0].Control & 0xFFFFE0FF) |
            (NHET1_PIN_PWM << 8);
}

void calculate_ecmp_compare()
{
    int i=0;
/* The number of sample points is fixed using SAMPLE_SIZE. The SINE
* wave frequency is F = 1 / ((SINE_FREQ_DIVIDER * LRP) * Samples).
*
* The way to generate a divided sine wave frequency is to increase
* the PWM base frequency inversely. As we change the PWM base frequency
* we also need to adjust the compare values to generate the PWM duty
* cycle at each sine wave sample point.
*/
    for (i=0;i<=SAMPLE_SIZE;i++)
    {
/* CNT_MAX_PERIOD * 128 gives the total number of high resolution
* clocks in one PWM_PERIOD. The multiplication is done by performing
* a left shift of CNT_MAX_PERIOD by 7 bits.
*/
        sine_table[i] = (sine_table_percent[i] * (CNT_MAX_PERIOD << 7));
    }

}


/* USER CODE END */



/* USER CODE BEGIN (4) */
/* USER CODE END */
