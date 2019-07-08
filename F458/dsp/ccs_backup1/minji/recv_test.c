#include <stdio.h>
#include <string.h>
#include <signal.h>
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
	int ret;
	//char buf[32] = "message\r\n";
	int len = strlen(buf);

	fd = serial_config(dev0);
	signal(SIGINT, call_exit);

	if(!(ret = setjmp(env)))
	{
		for(;;)
		{
			//send_data(fd, buf, len, 50);
			recv_data(fd);
		}
	}

	return 0;
}
