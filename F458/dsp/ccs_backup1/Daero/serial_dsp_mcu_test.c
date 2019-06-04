#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>

extern char *dev0;
jmp_buf env;
int fd;

void call_exit(int signo)
{
	close_dev(fd);
	longjmp(env, 1);
}

int main(void)
{
	int ret, nr;
	char buf[64] = "messass\r\n";
	int len = strlen(buf);

	fd = serial_config(dev0);
	signal(SIGINT, call_exit);

	if(!(ret = setjmp(env)))
	{
		for(;;)
		{
			nr = read(0, buf, sizeof(buf));
			buf[nr-1] = '\0';
			printf("buf = %s\n", buf);

			send_data(fd, buf, nr, 50);
			recv_data(fd);
		}
	}
}
