#include <stdio.h>
#include <stdlib.h>

void f23th(void)
{
	int i;
	int a[23] = {0};

	a[0] = 1;
	a[1] = 3;

	for(i=2; i<23; i++)
	{
		a[i] = a[i-2] + a[i-1];
	}
	printf("23th = %d\n", a[22]);
}

int main(void)
{
	f23th();

	return 0;
}
