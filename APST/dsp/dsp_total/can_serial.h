#ifndef __SERIAL_H__
#define __SERIAL_H__

void drv_chk(void);
void printf_can_arr(char *, int);
void can_set_tx_buf(char *, char *, char);
int can_serial_config(char *);
void can_send_data(int, char *, int, int);
void can_recv_data(int);
void can_close_dev(int);

#endif
