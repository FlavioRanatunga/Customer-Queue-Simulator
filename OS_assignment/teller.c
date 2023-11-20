/*Dequeues a customer from the customer queue and is served by the teller according to the service type, Also responsible for getting the count
for number of customers served after termination*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include "mutexes.h"
#include "queue.h"
#include "cq.h"
#include "teller.h"


void* teller(void* arg)
{
    FILE *r_log;
    time_t rawtime;/*time structs*/
    struct tm *now_tm;
    char buffer[256];
    tellerData* data = (tellerData*)arg;/*Getting teller data which are passed to the thread*/
    queue* q = data->customer_queue;
    int id = data->tellerId;
    char* startTime = data->arrival_time;
    int count = data->serv_count;
    customer_q customer;
    int end ;
    rawtime = time(NULL);
    now_tm = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", now_tm);/*Getting the start time of the teller thread*/
    strcpy(startTime, buffer);
    end = 0;
    count = 0;
    
    while(end == 0)/*loops until the flag is true and the is queue is empty*/
    {
        pthread_mutex_lock(&mutex);
        while(isEmpty(q))/*waiting until a customer is enqueued*/
        {
            if(flag)
            {
                end = 1;
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_cond_wait(&notEmpty, &mutex);/*Waiting until notEmpty is signaled by another thread*/
        }

        if(end == 0)
        {
        customer = dequeue(q);/*Dequeue customer from queue*/
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&notFull);/*notFull is signaled*/
        
        
        rawtime = time(NULL);/*Getting current time to log the response time of the teller*/
        now_tm = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", now_tm);

        pthread_mutex_lock(&logMutex);
        r_log = fopen("r_log.txt", "a");
        if (r_log == NULL) 
        {
            fprintf(stderr, "Error opening r_log file\n");
            exit(1);
        }
        fprintf(r_log, "\nTeller: %d\nCustomer#: %d\nArrival time: %s\nResponse time: %s\n", id, customer.customer_id, customer.arrival_time, buffer);
        /*Writing response time to the file*/
        fclose(r_log);
        pthread_mutex_unlock(&logMutex);/*file writing unlocked*/

        switch (customer.serv_type)/*Switch case to identify service type and using sleep time using arguments*/
        {
            case 'W':
                sleep(tw);
                break;
            case 'D':
                sleep(td);
                break;
            case 'I':
                sleep(ti);
                break;
        }
        count++; /*served customer count incremented*/
        rawtime = time(NULL);/*Getting current time to log completion of customer service*/
        now_tm = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", now_tm);
        pthread_mutex_lock(&logMutex);
        r_log = fopen("r_log.txt", "a");
        if (r_log == NULL) 
        {
            fprintf(stderr, "Error opening r_log file\n");
            exit(1);
        }
        fprintf(r_log, "\nTeller: %d\nCustomer#: %d\nArrival time: %s\nCompletion time: %s\n", id, customer.customer_id, customer.arrival_time, buffer);
        /*Writing completion of customer to r_log*/
        fclose(r_log);
        pthread_mutex_unlock(&logMutex);
        }
        
    }
    rawtime = time(NULL);/*Getting current time to get the termination time of the teller after exiting the while loop*/
    now_tm = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", now_tm);
    pthread_mutex_lock(&logMutex);
    r_log = fopen("r_log.txt", "a");
    if (r_log == NULL) 
    {
        fprintf(stderr, "Error opening r_log file\n");
        exit(1);
    }
    fprintf(r_log, "\nTermination: teller-%d\n#Served customers: %d\nStart time: %s\nTermination time: %s\n", id, count, startTime, buffer);
    /*Writing termination of teller*/
    fclose(r_log);
    pthread_mutex_unlock(&logMutex);
   
    pthread_cond_signal(&notEmpty);
    data->serv_count = count;
    return NULL;
}

