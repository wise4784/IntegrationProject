#ifndef DSP_TOTAL_H
#define DSP_TOTAL_H

#include <pthread.h>

extern pthread_mutex_t sync_mutex;
extern pthread_cond_t sync_cond;
extern pthread_cond_t sync_cond2;
extern int sync_flag;
#endif
