#include "can_serial.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

extern char data_buf[9];
extern char *can_dev0;

int main(void)
{
        int fd;

        fd = can_serial_config(can_dev0);

        for(;;)
        {
                can_recv_data(fd);
        }

        can_close_dev(fd);

        return 0;
}
