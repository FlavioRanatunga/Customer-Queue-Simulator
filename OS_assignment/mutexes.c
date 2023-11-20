/*To protect shared variables and avoid race conditions between threads.*/
#include "mutexes.h"

/*Initializing mutex and condtion variables*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t notEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t notFull = PTHREAD_COND_INITIALIZER;

