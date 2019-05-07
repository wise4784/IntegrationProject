#include <stdio.h>

float test(float a, float b)
{
	return a + b;
}

int main(void)
{
	float a = 0.33777, b = 0.37373, c;
	c = test(a, b);
	printf("c = %f\n", c);
	return 0;
}
