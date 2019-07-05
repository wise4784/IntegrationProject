#include "can_serial.h"
#include "vcp_can.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    init_can();

    while(1)
    {
        Recieve_fpga_data();
        usleep(10000);
    }

    close_can();

    return 0;

}
