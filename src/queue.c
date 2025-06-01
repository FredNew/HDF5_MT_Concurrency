#include "queue.h"

#include <stdlib.h>
#include <pthread.h>

int queue_add(queue* q, void* item){

    if (item == NULL)
    {
        pthread_mutex_lock(&q->lock);
        q->nelmts = -1;
        pthread_cond_signal(&q->wait);
        pthread_mutex_unlock(&q->lock);
        return 0;
    }
    queue_item* q_item = malloc(sizeof(queue_item));
    q_item->held_item = item;
    q_item->next = NULL;

    pthread_mutex_lock(&q->lock);

    if(q->head == NULL){
        q->head = q_item;
        q->tail = q_item;
        q->nelmts = 1;
        q->elmts_added++;
    }else{
        q->tail->next = q_item;
        q->tail = q_item;
        q->nelmts++;
        q->elmts_added++;
    }
    pthread_cond_signal(&q->wait);
    pthread_mutex_unlock(&q->lock);

    return 1;
}

ssize_t queue_get_elmts_added(queue* q){
  	ssize_t ret_val = -1;

  	pthread_mutex_lock(&q->lock);
  	ret_val = q->elmts_added;
    pthread_mutex_unlock(&q->lock);

    return ret_val;
}

void* queue_get(queue* q){

    void* ret_val;

    pthread_mutex_lock(&q->lock);

    while(q->nelmts == 0){ //using negative numbers to denote status done
        pthread_cond_wait(&q->wait, &q->lock);
    }

    if(q->head == NULL){
        ret_val =  NULL;
    }else{
        queue_item* q_item = q->head;
        ret_val = q_item->held_item;

        q->head = q->head->next;
        q->nelmts--;

        free(q_item);
    }
    pthread_mutex_unlock(&q->lock);
    
    return ret_val;
}
