#include "pos_pid.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int position_pid(int target_deg, int current_deg,int reset)
{

    static unsigned int pos_pid_cnt = 0
    int error =0;
    int d_error;
    static int error_sum = 0;
    static int error_old = 0;
    static int ouput_offset =0;
    int output;

    if(reset == 1)
    {
        error   =   target_deg-current_deg;
        error_sum +=    (error + error_old)/2;
        d_error = error - error_old;
    }
    else
    {
        error_sum =0;
        error_old = 0;
        ouput_offset =0;
    }

    output =(int)(PGAIN*(float)error +IGAIN*(float)error_sum+DGAIN*(float)d_error) + ouput_offset;

    pos_pid_cnt++;
    if(pos_pid_cnt>100000)
    {
        ouput_offset  = IGAIN*error_sum;
		  error_sum =0;
        pos_pid_cnt =0;
    }
    error_old = error;

    return output;
}
