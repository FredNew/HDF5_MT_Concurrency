#ifndef THREADPOOL_H
#define THREADPOOL_H

typedef struct thread_arguments{
    pthread_t thread_id;
    size_t thread_number;
    int status;

    void* application_args;
    void* (*application_function)(void* thread_args);
} thread_arguments;

void* thread_start(void* args);

int thread_work(thread_arguments* targs);

size_t get_thread_count(void);

#endif