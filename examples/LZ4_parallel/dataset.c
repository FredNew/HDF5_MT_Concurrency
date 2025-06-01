#include <stdlib.h>
#include <limits.h>
#include "dataset.h"

int* create_int_ds(unsigned long length){
    int* ret_val = malloc(length * sizeof(int));
    
    int counter = 0;
    for(int i = 0; i < length; i++){
        ret_val[i] = i%2 == 0? counter : 1;

        if(counter == INT_MAX){
            counter = 0;
        }
        else{
            counter++;
        }
    }

    return ret_val;
}

float* create_float_ds(int length){
    float* ret_val = malloc(length * sizeof(float));


    return ret_val;
}
