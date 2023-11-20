#ifndef TELLER_H
#define TELLER_H
#include "queue.h"


typedef struct 
{
    int tellerId;
    queue* customer_queue;
    char arrival_time[20];
    int serv_count;
} tellerData;

typedef struct 
{
    queue* customer_queue;
    
} customerData;

void* teller(void* arg);

#endif
