#include <stdio.h>
#include <unistd.h>

int main(void)
{
	execlp("ls", "ls", "-a", "-l", 0);
	printf("printing -l is succeeded\n");

	return 0;
}
