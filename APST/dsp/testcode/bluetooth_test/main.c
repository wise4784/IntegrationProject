#include "bluetooth.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    printf("init_start\r\n");
    init_bl();
    printf("init_success\r\n");

    while(1)
    {
        recieve_bl();

        if(read_bl_ins>0)
        {
            if(read_bl_ins==1)
                printf("left\n");
            else if(read_bl_ins==2)
                printf("up\n");
            else if(read_bl_ins==3)
                printf("right\n");
            else if(read_bl_ins==4)
                printf("down\n");
            else if(read_bl_ins==5)
                printf("laser\n");
            else if(read_bl_ins==6)
                printf("shoot\n");
            else if(read_bl_ins==7)
                printf("mode\n");
            else
                printf("start\n");
            read_bl_ins = 0;
        }
    }

    return 0;
}
