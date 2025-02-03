#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "myBarrier.h"

#define FAMILY_MEMBERS 6
#define DAYS_NUMBER 5

my_barrier_t breakfastEvent;
my_barrier_t mathEvent;
my_barrier_t playEvent;
my_barrier_t dinnerEvent;

typedef struct {
    char *name;
    my_barrier_t *specialEvent;
    char *morningActivity;
    char *afternoonActivity;
    char *specialEventActivity;
} FamilyMember;

void *routine(void *args) {
    FamilyMember *member = (FamilyMember *)args;
    
    printf("%s woke up\n", member->name);
    sleep(1 + rand() % 3); // între 1 și 3 secunde
    
    my_barrier_wait(&breakfastEvent);
    printf("%s %s\n", member->name, member->morningActivity);
    sleep(4 + rand() % 7); // între 4 și 7 secunde

    printf("%s %s\n", member->name, member->afternoonActivity);
    sleep(1 + rand() % 3); // între 1 și 3 secunde

    if (member->specialEvent) { 
        printf("%s %s\n", member->name, member->specialEventActivity); 
        my_barrier_wait(member->specialEvent);  
        sleep(1 + rand() % 3); // între 1 și 3 secunde
    }

    printf("%s is ready for dinner\n", member->name);
    my_barrier_wait(&dinnerEvent);
    printf("%s goes to sleep\n", member->name);
    sleep(5 + rand() % 10); // între 5 și 10 secunde

    return NULL;
}

int main(void) {
    pthread_t threadHandler[FAMILY_MEMBERS];

    my_barrier_init(&breakfastEvent, FAMILY_MEMBERS);
    my_barrier_init(&mathEvent, 2);
    my_barrier_init(&playEvent, 2);
    my_barrier_init(&dinnerEvent, FAMILY_MEMBERS);

    FamilyMember family[FAMILY_MEMBERS] = {
        {"Mama", NULL, "goes to work", "is ready for dinner", ""},
        {"Papa", NULL, "goes to work", "is ready for dinner", ""},
        {"Alice", &mathEvent, "goes to school", "goes home", "ready for math with Bob"},
        {"Bob", &mathEvent, "goes to school", "goes home", "ready for math with Alice"},
        {"Chris", &playEvent, "goes to school", "goes home", "ready to play with Dave"},
        {"Dave", &playEvent, "goes to school", "goes home", "ready to play with Chris"}
    };

    for (int i = 0; i < DAYS_NUMBER; i++) {
        printf("DAY %d simulation has started\n", i + 1);

        for (int j = 0; j < FAMILY_MEMBERS; j++) {
            if (pthread_create(&threadHandler[j], NULL, routine, &family[j]) != 0) {
                fprintf(stderr, "Error creating thread for %s\n", family[j].name);
                exit(EXIT_FAILURE);
            }
        }

        for (int j = 0; j < FAMILY_MEMBERS; j++) {
            if (pthread_join(threadHandler[j], NULL) != 0) {
                fprintf(stderr, "Error joining thread for %s\n", family[j].name);
                exit(EXIT_FAILURE);
            }
        }
    }

    my_barrier_destroy(&breakfastEvent);
    my_barrier_destroy(&mathEvent);
    my_barrier_destroy(&playEvent);
    my_barrier_destroy(&dinnerEvent);

    return 0;
}
