#ifndef QUEUE_H
#define QUEUE_H

typedef struct
{
    int customer_id;
    char serv_type;
    char arrival_time[20];
}customer_q;

typedef struct node_q
{
    customer_q customer;
    struct node_q* next;
}node_q;

typedef struct
{
    node_q* front;
    node_q* rear;
    int count;
    int size;
}queue;

extern bool flag;

void createQueue(queue* q, int size);
bool isEmpty(queue* q);
bool isFull(queue* q);
void enqueue(queue* q, customer_q customer);
customer_q dequeue(queue* q);
void* customer(void* arg);
void deleteQueue(queue* q);

#endif

