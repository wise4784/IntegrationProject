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
        int fd;

        fd = serial_config(dev0);

        for(;;)
        {
                recv_data(fd);
        }

        close_dev(fd);

        return 0;
}
