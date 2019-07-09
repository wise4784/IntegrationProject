#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <pthread.h>

extern char read_bl_ins;
pthread_mutex_t bl_mutx;

void init_bl(void);
void recieve_bl(void);
void close_bl_dv(void);

#endif
