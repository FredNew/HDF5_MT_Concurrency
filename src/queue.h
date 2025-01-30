#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct queue_item{
    void* next;

    void* held_item;
} queue_item;

typedef struct queue{
    int nelmts;
    int elmts_added;

    queue_item* head;
    queue_item* tail;

    pthread_mutex_t lock;
    pthread_cond_t wait;

} queue;

int queue_add(queue* q, void* item);

void* queue_get(queue* q);

#endif