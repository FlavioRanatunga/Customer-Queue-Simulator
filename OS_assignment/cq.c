#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "teller.h"
#include "mutexes.h"
#include "queue.h"
#include "cq.h"

#define NTELLERS 4
int tc, tw, td, ti;

int main(int argc, char *argv[])
{
    FILE *r_log;
    pthread_t customer_thread;
    pthread_t teller_threads[NTELLERS];
    tellerData tData[NTELLERS];
    customerData cData;
    int i;
    int sTotal;
    queue customer_queue;
    int m;

    r_log = fopen("r_log.txt", "w"); /*Erase logs from previous execution*/
    if (r_log == NULL) 
    {
        printf("Error opening file\n");
        exit(1);
    }
    fclose(r_log); 

    if (argc != 6) {
        printf("Use format: %s m tc tw td ti\n", argv[0]);/*aborted if arguments not equal to 6*/
        return 1;
    }

    m = atoi(argv[1]);
    tc = atoi(argv[2]);
    tw = atoi(argv[3]);
    td = atoi(argv[4]);
    ti = atoi(argv[5]);

    if (m <= 0 || tc <= 0 || tw <= 0 || td <= 0 || ti <= 0) /*Check if arguments are positive*/
    {
        printf("Error: All integers must be positive.\n");
        return 1;
    }

    /*Creating with size m queue*/
    createQueue(&customer_queue, m);
    /*Initializing customer and teller data*/
    cData.customer_queue = &customer_queue;
    tData[0].customer_queue = &customer_queue;
    tData[1].customer_queue = &customer_queue;
    tData[2].customer_queue = &customer_queue;
    tData[3].customer_queue = &customer_queue;
    

    pthread_create(&customer_thread, NULL, customer, &cData);/*Cusomter thread created*/
    for (i = 0; i < NTELLERS; i++) 
    {
        tData[i].tellerId = i + 1; 
        pthread_create(&teller_threads[i], NULL, teller, &tData[i]);/*Creating 4 teller threads*/
    }

    /*Wating for threads to terminate*/
    pthread_join(customer_thread, NULL);
    pthread_join(teller_threads[0], NULL);
    pthread_join(teller_threads[1], NULL);
    pthread_join(teller_threads[2], NULL);
    pthread_join(teller_threads[3], NULL);

    sTotal = 0;

    r_log = fopen("r_log.txt", "a");/*Logging the Teller statistics after all tellers are done*/
    fprintf(r_log, "\nTeller Statistics\n");
    for (i = 0; i < NTELLERS; i++) 
    {
        fprintf(r_log, "Teller-%d serves %d customers.\n", tData[i].tellerId, tData[i].serv_count);
        sTotal += tData[i].serv_count;
    }
    fprintf(r_log,"Total number of customers: %d.\n", sTotal);
    fclose(r_log);

    deleteQueue(&customer_queue);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&notEmpty);
    pthread_cond_destroy(&notFull);

    return 0;
}