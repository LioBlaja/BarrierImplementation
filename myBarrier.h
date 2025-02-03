#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct {
    int count; 
    int executionCounter; 
    int usageCount;
    pthread_mutex_t mutex; 
    pthread_cond_t cond; 
} my_barrier_t;

void my_barrier_init(my_barrier_t *b, int count);

void my_barrier_destroy(my_barrier_t *b);

void my_barrier_wait(my_barrier_t *b);