#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>

jmp_buf env;
extern char *dev0;
int fd;

void call_exit(int signo)
{
    close_dev(fd);
    longjmp(env,1);
}

/*
void set_label(void)
{
    for(;;)
        recv_data(fd);
}
*/

int main(void)
{
    int ret;
    char buf[32];
    char *gpio[2] = {"gpio=1","gpio=0"};
    int len ;

  fd = serial_config(dev0);
    signal(SIGINT,call_exit);

    if( !(ret = setjmp(env)) )
    {
        for(;;)
        {
            len = read(0,buf,sizeof(buf));
            printf("%d\n",len);

            if( !strncmp(buf,gpio[0],6))
                send_data(fd,buf,6,50);
            else if( !strncmp(buf,gpio[1],6))
                send_data(fd,buf,6,50);
            else
                ;
        }
    }
    return 0;
}
