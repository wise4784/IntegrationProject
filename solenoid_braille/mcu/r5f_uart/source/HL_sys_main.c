/** @file HL_sys_main.c
*   @brief Application main file
*   @date 07-July-2017
*   @version 04.07.00
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
#include "HL_system.h"
#include "HL_sci.h"
#include "HL_gio.h"

#define UART    sciREG1

void sci_display(sciBASE_t *sci, uint32 *text, uint32 len);
uint32 sci_receive(sciBASE_t *sci);
void sci_scanf(sciBASE_t *sci, uint32 *rev_data);
uint32 UTF8toUnicode(uint32 *s);
bool chkhan(const char ch);
bool chkHanUnicode(const uint32 *ch);
uint8 BreakHan(uint32 *str, uint32 *buffer, uint8 nSize);
void DivideHan(uint8 pos);
void check_msg(uint32 *data);
//void wait(int delay);

uint32 han_ch [999] = {0};
uint32 han_ch2 [999] = {0};
uint32 buffer [999] = {0};
uint32 buffer2 [999] = {0};
uint8 data_buffer [999] = {0};

uint8 pos = 0;



static uint32 wcF[] = {
    'ㄱ', 'ㄲ', 'ㄴ', 'ㄷ',
    'ㄸ', 'ㄹ', 'ㅁ', 'ㅂ',
    'ㅃ', 'ㅅ', 'ㅆ', 'ㅇ',
    'ㅈ', 'ㅉ', 'ㅊ', 'ㅋ',
    'ㅌ', 'ㅍ', 'ㅎ'};

//중성
static uint32 wcM[] = {
    'ㅏ', 'ㅐ', 'ㅑ', 'ㅒ',
    'ㅓ', 'ㅔ', 'ㅕ', 'ㅖ',
    'ㅗ', 'ㅘ', 'ㅙ', 'ㅚ',
    'ㅛ', 'ㅜ', 'ㅝ', 'ㅞ',
    'ㅟ', 'ㅠ', 'ㅡ', 'ㅢ', 'ㅣ'};

//종성
static uint32 wcL[] = {
    ' ', 'ㄱ', 'ㄲ', 'ㄳ',
    'ㄴ', 'ㄵ', 'ㄶ', 'ㄷ',
    'ㄹ', 'ㄺ', 'ㄻ', 'ㄼ',
    'ㄽ', 'ㄾ', 'ㄿ', 'ㅀ',
    'ㅁ', 'ㅂ', 'ㅄ', 'ㅅ',
    'ㅆ', 'ㅇ', 'ㅈ', 'ㅊ',
    'ㅋ', 'ㅌ', 'ㅍ', 'ㅎ'};
/* USER CODE END */


int main(void)
{
/* USER CODE BEGIN (3) */

    int i, j, l;
    int regwait = 0;
    uint8 k = 0;
    sciInit();
    gioInit();

    while(1)
    {
       sci_scanf(UART, han_ch2); // 데이터 입력

       for(i = 0; han_ch2[i]; i++)
       {
          // printf("han_ch2[%d] = %d, %#X\n",i, han_ch2[i], han_ch2[i]);
           if(chkhan(han_ch2[i]))
           {
              // printf("if 1\n");
               han_ch[i] = UTF8toUnicode(&han_ch2[i]); //한글 UTF8 - Unicode 변환]

               //printf("if 2\n");
               k  = BreakHan(&han_ch[i], buffer, sizeof(buffer));


               //printf("if 3\n");
               /*printf("F = %#X\n", buffer[0]);
               printf("M = %#X\n", buffer[1]);
               printf("L = %#X\n", buffer[2]);

               printf("Han_ch0 = %#X\n", han_ch[0]);*/

               //F, M, L 각 3바이트 한 바이트씩 나누기
               //DivideHan(i);

               /*
               for(i = 0; buffer[i]; i++) // 한글 바이트 카운트
                           ;
               sci_display(UART, buffer, i);// 출력
               */

               for(j = 0; han_ch2[j]; j++) // 한글 바이트 카운트
                    ;
               sci_display(UART, han_ch2, j);// 출력
               i+=2;
           }
           else
           {
               sci_display(UART, &han_ch2[i], 1);
               buffer[pos] = han_ch2[i];
               pos+=1;
               k = pos-1;
           }
       }

       //printf("pos = %d\n", pos);

       if(pos > 0 && pos <= 7)
       {
           for(j = 0; j < k; j++)  // 총 6바이트씩 출력 반복
           {
              // printf("ch_buffer[%d] = %#X\n", j, buffer[j]);
               check_msg(&buffer[j]);  // 초,중,종 분리해서 넣어야 될 필요성!(초성 종성 같은 ㄱ 구분 불가) %연산 이용 j
           }
       }
       else if(pos > 7) // 6 바이트 출력후 뒤에 바이트 땡겨서 출력 반복 // for문으로 처리하면 안됌 바로 넘어가기 때문에 여기다가 스위치 응답 받을경우 넘어가도록
       {
           for(j = 0; j < k; j+=6)     // if pos k가 9면 각각각  4글자 경우(0~11)
           {
               if(regwait)
                   wait(40000000);         // 스위치 대신 우선은 wait 함수로 대기시간(read time)을 준다.
               for(l = j; l < j+6; l++)     // 1. l = 0 ~ 5 까지 check / 2. l= 6 ~ 11
               {
                   regwait=1;
                   if(buffer[l] || buffer[l] != "\n" || buffer[l] != "\r" || buffer[l] != "\0")
                       check_msg(&buffer[l]);
               }
           }
           regwait = 0;
       }

       memset(han_ch2, 0, sizeof(han_ch2)); // 버퍼 초기화
       memset(han_ch, 0, sizeof(han_ch)); // 버퍼 초기화
       memset(buffer, 0, sizeof(buffer)); // 버퍼 초기화
       pos= 0;
       k = 0;

    }
/* USER CODE END */

    return 0;
}

/* USER CODE BEGIN (4) */
void sci_display(sciBASE_t *sci, uint32 *text, uint32 len)
{
    while(len--)
    {
        while((UART->FLR & 0x4) == 4)
            ;

        while ((sci->FLR & (uint32)SCI_TX_INT) == 0U)
            ; /* Wait */
        sci->TD = *text++;
    }
}

uint32 sci_receive(sciBASE_t *sci)
{
    while ((sci->FLR & (uint32)SCI_RX_INT) == 0U)
            ; /* WAIT */

    return (sci->RD & (uint32)0x00FFFFFFU); // return (uint32)RD[0~7]
}

#if 1
void sci_scanf(sciBASE_t *sci, uint32 *rev_data)
{
    int idx = 0;

    while(1)
    {
        rev_data[idx] = sci_receive(sci);
        if(rev_data[idx] == '\n' || rev_data[idx] == '\r')
            break;
        idx++;
    }
    idx = 0;


}
#endif

#if 0
void DivideHan(uint8 pos)
{
    int i, j;
    for(i = 0; buffer[i]; i++)
    {
        char c = buffer[i];
        for(j =0; j<pos; j += 3)
        {
            buffer2[j] = (buffer[i] & 0xFF0000) >> 16;
            printf("buffer2[0] = %d, %#X\n", buffer2[0], buffer2[0]);

            buffer2[j+1] = (buffer[i] & 0x00FF00) >> 8;
            printf("buffer2[0] = %d, %#X\n", buffer2[1], buffer2[1]);

            buffer2[j+2] = (buffer[i] & 0x0000FF);
            printf("buffer2[0] = %d, %#X\n", buffer2[2], buffer2[2]);
        }
    }
}
#endif

uint32 UTF8toUnicode(uint32 *s)
{
    char c = s[0];
    uint32 ws = 0, wc = 0;

    /*Debug*/
    //printf("\n");
    //printf("s[0] = %d %#X\n", s[0], s[0]);
    //printf("s[1] = %d %#X\n", s[1], s[1]);
    //printf("s[2] = %d %#X\n", s[2], s[2]);
    /*End*/

    if((c & 0xE0) == 0xE0)
    {
        wc = (s[0] & 0x0F) << 12;
       // printf("s[0] = %d, %#X\n", wc, wc);

        wc |= (s[1] & 0x3F) << 6;
      //  printf("s[1] = %d, %#X\n", wc, wc);

        wc |= (s[2] & 0x3F);
        //printf("s[2] = %d, %#X\n", wc, wc);


    }
    ws += wc;

   // printf("ws = %d, %#X\n", ws , ws);

    return ws;

}

#if 1
bool chkhan(const char ch)              // for double byte code
{
   // printf("ch = %#X\n", ch);
        if((ch & 0x80) == 0x80)
        {
          //  printf("ch & 0x80 = %#X\n", ch & 0x80);
          //  printf("true\n");
            return  true; // 최상위 비트 1 인지 검사
        }
        else
        {
          //  printf("false\n");
            return false;
        }
}

bool chkHanUnicode(const uint32 *ch) // for unicode
{
    return !(ch < 44032 || ch > 55199); // 0xAC00(가) ~ 0xD7A3(힣)
}
#endif

// 초성 중성 종성 분리
uint8 BreakHan(uint32 *str, uint32 *buffer, uint8 nSize)
{


/*
    while(*str != '\0' || *str != '\r' || *str != '\n')
    {

        if(*str < 256)
        {
            if(pos+2 >= nSize-1)
                break;

            buffer[pos] = *str;
            ++pos;
        }
        else
        {
            if(pos+4 >= nSize-1)
                break;
         */
           // printf("str = %#X\n", *str);

            buffer[pos] = wcF[(*str - 0xAC00) / (21*28)];
          //  printf("a : %d\n", (*str - 0xAC00) / (21*28));
          //  printf("buffer0[%d] = %#X\n", pos, buffer[pos]);

            buffer[pos+1] = wcM[(*str - 0xAC00) % (21 * 28) / 28];
           // printf("b : %d\n", (*str - 0xAC00) % (21 * 28) / 28);
           // printf("buffer1[%d] = %#X\n", pos+1, buffer[pos+1]);

            buffer[pos+2] = wcL[(*str - 0xAC00) % 28];
          //  printf("c : %d\n", (*str - 0xAC00) % 28);
          //  printf("buffer2[%d] = %#X\n", pos+2, buffer[pos+2]);

            pos+=3;
       // }

        //++str;
    //}

    //buffer[pos] = '\0';
    return pos;
}

void check_msg(uint32 *data)
{
   // printf("check data = %#X\n", *data);
    if( (*data) == 'ㄱ')
    {
        //printf("led 1 on\n");
        gioSetBit(gioPORTA, 1, 1);
    }

    else if( (*data) == 'ㅏ')
    {
       // printf("led 2 on\n");
        gioSetBit(gioPORTA, 2, 1);
    }

    else if( (*data) == 'ㄼ')
    {
        //printf("led 3 on\n");
        gioSetBit(gioPORTA, 3, 1);
    }

    else if( (*data) == 'ㄴ')
    {
        //printf("led 4 on\n");
        gioSetBit(gioPORTA, 4, 1);
    }

    else if( (*data) == 'ㅑ')
    {
      //  printf("led 5 on\n");
        gioSetBit(gioPORTA, 5, 1);
    }

    else if( (*data) == 'ㅇ')
    {
       // printf("led 6 on\n");
        gioSetBit(gioPORTA, 6, 1);
    }

    else
    {
        gioSetBit(gioPORTA, 1, 0);
        gioSetBit(gioPORTA, 2, 0);
        gioSetBit(gioPORTA, 3, 0);
        gioSetBit(gioPORTA, 4, 0);
        gioSetBit(gioPORTA, 5, 0);
        gioSetBit(gioPORTA, 6, 0);
    }
}

#if 1
void wait(int delay)
{
    int i;
    for(i = 0; i < delay; i++)
        ;
}
#endif
/* USER CODE END */
