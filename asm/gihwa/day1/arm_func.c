#include <stdio.h>

int mult(int num)
{
	return num * 2;
}

int main(void)
{
	int num=3, res;
	res=mult(num);
	printf("res=%d\n", res);
	return 0;
}
