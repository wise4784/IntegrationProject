#include <stdio.h>

unsigned int arr[6] = {1, 2, 3, 4, 5};

int main(void)
{
	register unsigned int r0 asm("r0") = 0;
	register unsigned int *r1 asm("r1") = 0;// r1을 주소값을 받는 레지스터로로 사용.
	register unsigned int r2 asm("r2") = 0;

	r1 = arr;

	asm volatile(	"mov r2, #0x8\r\n"
					"ldr r0, [r1, r2]");//배열의 이름은 시작주소. 3을 r0넣음

	printf("r0 = %u\n", r0); //3이나옴...

	return 0;
}

