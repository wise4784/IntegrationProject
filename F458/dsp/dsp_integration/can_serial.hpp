#ifndef __SERIAL_H__
#define __SERIAL_H__

extern "C" {

void drv_chk(void);
void printf_can_arr(char *, int);
void set_bldc_tx_buf(char *, char *, char);
int serial_config(char *);
void send_data(int, char *, int, int);
void recv_data(int);
void close_dev(int);
}
#endif
