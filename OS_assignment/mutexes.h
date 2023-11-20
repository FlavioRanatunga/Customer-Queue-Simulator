#ifndef MUTEXES_H
#define MUTEXES_H

#include <pthread.h>

extern pthread_mutex_t mutex;
extern pthread_cond_t notEmpty;
extern pthread_cond_t notFull;
extern pthread_mutex_t logMutex;


#endif