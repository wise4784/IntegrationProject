#include "vcp_can.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int can_success =0;
    init_can();

    while(1)
    {
        can_success = Recieve_fpga_data();
        if(can_success == -1)
            usleep(5);
        else
        {
            printf("lidar = %d\t", lidar_data);
            printf("encoder = %d\n",encoder_data);

            lidar_data = -1;
            encoder_data = -1;
            usleep(10000);
        }
    }

    close_can();

    return 0;

}
