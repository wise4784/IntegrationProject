#include "can_serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

extern char data_buf[9];
extern char *dev0;

int main(void)
{
        int i;
        int fd;

        fd = serial_config(dev0);

        for(;;)
        {
                recv_data(fd);
                printf("can we look data_buf ? \n");

                for(i = 0; i < 8; i++)
                        printf("%d", data_buf[i]);

                printf("\n");
        }

        close_dev(fd);

        return 0;
}
