#include <stdio.h>

int mult(int num,int a,int b,int c,int d)
{
	a=1;
	b=2;
	c=3;
	d=4;
	return num+a+b+c+d;
}

int main(void)
{
	int num = 3, res;
	int a=0;
	int b=0;
	int c=0;
	int d=0;
	res = mult(num,a,b,c,d);
	printf("res = %d\n",res);

	return 0;
}
