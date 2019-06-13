#include <stdio.h>



int func(int a, int b)
{
	return a+b;
}
int main(void)
{
	int num,res;
	int a=3,b=4;
	int (*add)(int,int) = func;
	res = add(a,b);
	

	return 0;
}
