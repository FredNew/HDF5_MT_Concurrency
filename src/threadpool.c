#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>

void* thread_start(void* args){
    thread_arguments* targs = (thread_arguments*) args;

    thread_work(targs);

    return (void*) 0;
}

int thread_work(thread_arguments* targs){
    targs->application_function(targs);

    return 0;
}

size_t get_thread_count(void){
    size_t nthreads = 1;

    int env_idx = 0;
    long tmp_add = 0;

    while(getenv("H5_NTHREADS") != NULL && getenv("H5_NTHREADS")[env_idx]) { //string of characters terminating with \0
         if (env_idx == 0) { //set initial
            nthreads = 0;
        }

        nthreads *= 10; //
        tmp_add = getenv("H5_NTHREADS")[env_idx] - 48; //converting ascii to int

        if (tmp_add < 0 || tmp_add > 9) {
            fprintf(stderr, "H5_NTHREADS not valid.\n");
            nthreads = 1;
            break;
        }

        nthreads += (size_t) tmp_add;
        env_idx++;
    }

    return nthreads;
}


