#include "myBarrier.h"

void my_barrier_init(my_barrier_t *b, int count){
    b->count = count;
    b->executionCounter = 0;
    // b->executionCounter = 0;
    b->usageCount = 0;
    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cond, NULL);
}

void my_barrier_destroy(my_barrier_t *b){
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cond);
}

void my_barrier_wait(my_barrier_t *b){
    pthread_mutex_lock(&b->mutex);

    int localExecutionCounter = b->usageCount;
    b->executionCounter++;
    if(b->executionCounter == b->count){
        b->usageCount++;
        b->executionCounter = 0;
        pthread_cond_broadcast(&b->cond);
    }else{
        while(localExecutionCounter == b->usageCount)
            pthread_cond_wait(&b->cond,&b->mutex);
    }

    pthread_mutex_unlock(&b->mutex);
}