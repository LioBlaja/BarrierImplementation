#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "myBarrier.h"

// #define NUMTHREADS 10
int NUMTHREADS = 10;
int REPEATS = 10;
// #define REPEATS 10

my_barrier_t customBarrier;

pthread_barrier_t defaultBarrier;

struct timespec wallClockStart, wallClockFinish, cpuTimeStart, cpuTimeFinish;

void measureStartTime(clockid_t clockType){
    if (clockType == CLOCK_MONOTONIC) {
        // printf("Measuring Wall-clock time\n");
        // printf("Start ...\n");
        clock_gettime(clockType, &wallClockStart);
        // printf("Start time for wallClock: %ld seconds and %ld nanoseconds\n", wallClockStart.tv_sec, wallClockStart.tv_nsec);
    } else if (clockType == CLOCK_PROCESS_CPUTIME_ID) {
        // printf("Measuring CPU usage time\n");
        // printf("Start ...\n");
        clock_gettime(clockType, &cpuTimeStart);
        // printf("Start time for CPU usage: %ld seconds and %ld nanoseconds\n", cpuTimeStart.tv_sec, cpuTimeStart.tv_nsec);
    } else {
        printf("=====>WRONG clockType\n");
    }
}

void measureFinishTime(clockid_t clockType){
     if (clockType == CLOCK_MONOTONIC) {
        // printf("Measuring Wall-clock time\n");
        // printf("Finish ...\n");
        clock_gettime(clockType, &wallClockFinish);
        // printf("Finish time for wallClock: %ld seconds and %ld nanoseconds\n", wallClockFinish.tv_sec, wallClockFinish.tv_nsec);
    } else if (clockType == CLOCK_PROCESS_CPUTIME_ID) {
        // printf("Measuring CPU usage time\n");
        // printf("Finish ...\n");
        clock_gettime(clockType, &cpuTimeFinish);
        // printf("Finish time for CPU usage: %ld seconds and %ld nanoseconds\n", cpuTimeFinish.tv_sec, cpuTimeFinish.tv_nsec);
    } else {
        printf("=====>WRONG clockType\n");
    }
}

double computeTimeDifference(clockid_t clockType){
    double difference = 0.0;

    if (clockType == CLOCK_MONOTONIC) {
        difference = (wallClockFinish.tv_sec - wallClockStart.tv_sec);
        difference += (wallClockFinish.tv_nsec - wallClockStart.tv_nsec) / 1000000000.0;
    } else if (clockType == CLOCK_PROCESS_CPUTIME_ID) {
        difference = (cpuTimeFinish.tv_sec - cpuTimeStart.tv_sec);  // Changed: Using cpuTimeFinish and cpuTimeStart for CPU time
        difference += (cpuTimeFinish.tv_nsec - cpuTimeStart.tv_nsec) / 1000000000.0;
    }

    return difference;
}

void *fct1(void* args){
    for (int i = 0; i < REPEATS; i++) {
        sleep(1 + rand() % 2); // între 1 și 2 secunde
        pthread_barrier_wait(&defaultBarrier);
    }
    return NULL;
}

void *fct2(void* args){
    for (int i = 0; i < REPEATS; i++) {
        sleep(1 + rand() % 2); // între 1 și 2 secunde
        my_barrier_wait(&customBarrier);
    }
    return NULL;
}

float getAverageValues(float* arr, int size){
    float sum = 0.0;
    for(int i = 0; i < size;i++){
        sum += arr[i];
    }
    return sum / size;
}

int main(void){

    int threadsNumber[] = {2, 4, 8, 10};
    int repeatsNumber[] = {10, 20, 30, 40};

    int measurementsCount = 10;

    float valuesDefaultBarrier[measurementsCount];
    float valuesCustomBarrier[measurementsCount];

    for(int a = 0;a < 4;a++){
        NUMTHREADS = threadsNumber[a];
        REPEATS = repeatsNumber[a];

        for(int u = 0;u < measurementsCount;u++){
            printf("MEASUREMENT NO %d:%d\n",a + 1,u + 1);
            printf("============\n");
            printf("THREADS NUMBER:%d\n",NUMTHREADS);
            printf("REPEATS NUMBER:%d\n",REPEATS);
            pthread_t threadHandler[NUMTHREADS];

            my_barrier_init(&customBarrier, NUMTHREADS);
            pthread_barrier_init(&defaultBarrier,NULL, NUMTHREADS);

            measureStartTime(CLOCK_MONOTONIC);
            for(int i = 0;i < NUMTHREADS;i++){
                if (pthread_create(&threadHandler[i], NULL, fct1, NULL) != 0) {
                    fprintf(stderr, "Error creating thread");
                    exit(EXIT_FAILURE);
                }
            }
            for (int j = 0; j < NUMTHREADS; j++) {
                if (pthread_join(threadHandler[j], NULL) != 0) {
                    fprintf(stderr, "Error joining thread");
                    exit(EXIT_FAILURE);
                }
            }

            measureFinishTime(CLOCK_MONOTONIC);
            valuesDefaultBarrier[u] = computeTimeDifference(CLOCK_MONOTONIC);
            printf("The default barrier execution time:%f\n",valuesDefaultBarrier[u]);

            measureStartTime(CLOCK_MONOTONIC);
            for(int i = 0;i < NUMTHREADS;i++){
                if (pthread_create(&threadHandler[i], NULL, fct2, NULL) != 0) {
                    fprintf(stderr, "Error creating thread");
                    exit(EXIT_FAILURE);
                }
            }
            for (int j = 0; j < NUMTHREADS; j++) {
                if (pthread_join(threadHandler[j], NULL) != 0) {
                    fprintf(stderr, "Error joining thread");
                    exit(EXIT_FAILURE);
                }
            }

            measureFinishTime(CLOCK_MONOTONIC);
            valuesCustomBarrier[u] = computeTimeDifference(CLOCK_MONOTONIC);
            printf("The custom barrier execution time:%f\n",valuesCustomBarrier[u]);

            pthread_barrier_destroy(&defaultBarrier);
            my_barrier_destroy(&customBarrier);
        }

        float avg1 = getAverageValues(valuesDefaultBarrier,measurementsCount);
        printf("Average execution time for default barrier after %d measurements:%f\n",measurementsCount,avg1);
        float avg2 = getAverageValues(valuesCustomBarrier,measurementsCount);
        printf("Average execution time for custom barrier after %d measurements:%f\n",measurementsCount,avg2);
        printf("Difference between default barrier exec time and custom barrier exec time:%f\n",(avg1 - avg2));

    }

    return 0;
}
// Output example
// Average execution time for default barrier after 10 measurements:20.008625
// Average execution time for custom barrier after 10 measurements:20.012526
// Difference between default barrier exec time and custom barrier exec time:-0.003901