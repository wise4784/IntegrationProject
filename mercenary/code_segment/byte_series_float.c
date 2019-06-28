#include <stdio.h>

void set_arr2float(unsigned char *arr, unsigned char *num)
{
	int i;

	for(i = 0; i < 4; i++)
		num[i] = arr[i];
}

void set_float2arr(unsigned char *num, unsigned char *farr)
{
	int i;

	for(i = 0; i < 4; i++)
		farr[i] = num[i];
}

int main(void)
{
	unsigned char arr[5] = {0x13, 0x32, 0x27, 0x13};
	unsigned char farr[5] = {0};
	float num;
	int i;

	set_arr2float(arr, &num);

	printf("float num = %.64f\n", num);

	set_float2arr(&num, farr);

	for(i = 0; i < 4; i++)
		printf("farr[%d] = 0x%x\n", i, farr[i]);

	return 0;
}
