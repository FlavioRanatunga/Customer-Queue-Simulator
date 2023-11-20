/*Periodically gets a customer from c_file and enqueues it to the customer queue. Consists structs of the queue, node and customer data*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "teller.h"
#include "mutexes.h"
#include "cq.h"
#include "queue.h"

bool flag = false; /*flag to indicate when all customers are enqueued from the file*/

void createQueue(queue* q, int size)/*Function to create queue*/
{
    q->front = q->rear = NULL;
    q->count = 0;
    q->size = size;
}

bool isEmpty(queue* q)/*checks if queue is empty*/
{
    return q->count == 0;
}

bool isFull(queue* q)/*checks if queue is full*/
{
    return q->count == q->size;
}

void enqueue(queue* q, customer_q customer)/*adding customer to queue*/
{
    node_q* node;
    node = (node_q*)malloc(sizeof(node_q));
    node->customer = customer;
    node->next = NULL;

    if(isEmpty(q))
    {
        q->front = node;
        q->rear = node;
    }
    else
    {
        q->rear->next = node;
        q->rear = node;
    }
    q->count++;
}

customer_q dequeue(queue* q)/*function to remove customer*/
{
    customer_q customer;
    node_q * tmp;
    customer = q->front->customer;
    tmp = q->front;
    q->front = q->front->next;

    if(isEmpty(q))
    {
        q->rear = NULL;
        q->front = NULL;
    }
    free(tmp);
    q->count--;
    return customer;
}

void* customer(void* arg)/*Function that simulates customer when added to the queue*/
{
    customerData* data;
    queue* q;
    FILE *c_file;
    FILE *r_log;
    customer_q customer;
    char buffer[256];
    time_t rawtime;
    struct tm *now_tm;
    data = (customerData*)arg;
    q = data->customer_queue;

    c_file = fopen("c_file.txt", "r"); /*Opening customer file*/
    if (c_file == NULL)
    {
        fprintf(stderr, "Error opening customer file\n");
        exit(1);
    }

    /*Reading file until there are no more lines*/
    while (fscanf(c_file, "%d %c", &customer.customer_id, &customer.serv_type) == 2) 
    {
        sleep(tc);
        pthread_mutex_lock(&mutex);
        while(isFull(q)) /*Thread waits if queue is full*/
        { 
            pthread_cond_wait(&notFull, &mutex);
        }
        
        rawtime = time(NULL);
        now_tm = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%H:%M:%S", now_tm);
        strcpy(customer.arrival_time, buffer);

        enqueue(q, customer);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&notEmpty);
        
        pthread_mutex_lock(&logMutex);
        r_log = fopen("r_log.txt", "a");
        if (r_log == NULL) 
        {
            fprintf(stderr, "Error opening r_log file\n");
            exit(1);
        }
        fprintf(r_log, "\nCustomer#: %d Service type: %c\nArrival time: %s\n", customer.customer_id, customer.serv_type, customer.arrival_time);
        /*Writing arrival of customer to r_log*/
        fclose(r_log);
        pthread_mutex_unlock(&logMutex);
        
    }
    flag = true;/*flag set to true when all customers have been added to queue*/
    pthread_cond_signal(&notEmpty);
    fclose(c_file);
    return NULL;
}

void deleteQueue(queue* q)/*function to delete queue*/
{
    node_q* temp;

    while(q->front != NULL)/*traverse queue*/
    {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
    q->count = 0;
    q->size = 0;
}














