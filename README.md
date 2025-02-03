# BarrierImplementation
Own barrier implementation for threads sync

# Assignment1 - Variant: Barrier Implementation

## Cerințe pentru finalizare

This variant for Assignment1 allows a max of 20 points.

### Problem description

A barrier is a synchronization primitive that blocks all participants at a synchronization point until all participants reach that point.

The POSIX standard does define a barrier interface for Pthreads. However, some implementations of Pthreads provide barriers while others don’t provide barriers, so if our code is to be portable, we need to develop our own implementation.

In this assignment, you will implement your own barrier synchronization primitive. Your barrier implementation will implement an API very similar to pthreads:

- `pthread_barrier_t`   →   `my_barrier_t`
- `pthread_barrier_init` →   `my_barrier_init`
- `pthread_barrier_destroy` →   `my_barrier_destroy`
- `pthread_barrier_wait` →   `my_barrier_wait`

### Requirements

Implement the following API of a barrier synchronization primitive:

```c
typedef struct {
   int count; // counts threads in barrier

   // … add other fields that are necessary for your implementation
} my_barrier_t;
```

void my_barrier_init(my_barrier_t *b, int count);
void my_barrier_destroy(my_barrier_t *b);
void my_barrier_wait(my_barrier_t *b);
In order to use your barrier in programs, one needs to declare a my_barrier_t variable and initialize it with my_barrier_init(). The function my_barrier_init() takes the number of threads that will be participating in the barrier as an argument. The function my_barrier_wait() implements the synchronization point. Each thread that participates in the barrier must call the function my_barrier_wait() at the desired synchronization point. If the barrier has been initialized with count=N, then the first N-1 threads calling my_barrier_wait are blocked, until the last thread calls my_barrier_wait. After this, all threads of the barrier are unblocked and continue their execution. The barrier is back at ready to be used again.

You can use pthreads mutex locks and condition variables for your implementation. The barrier implementation will maintain a counter of threads reaching the barrier. This counter is a shared variable. A call to my_barrier_wait changes the counter, and if it is not the last thread, it starts waiting for a signal that the last thread arrived. If the counter shows that it was the last thread, the signal is sent.

Demonstration
Demonstrate the use of your barrier implementation. Use several variables of my_barrier_t type:

Create a multithreaded model of the daily life of Jones Family: Mama Jones, Papa Jones, Alice, Bob, Chris and Dave. Every member of the family is represented by a thread. Each thread knows the name of the family member it is representing and executes its daily schedule. Every family member has specific individual activities but there are also activities that require the synchronization of the whole family or just a part of the family members. Use your implementation of a barrier to ensure these synchronization points.

The daily schedule of the Jones is the following:

In the morning after waking up, all wait for each other to have breakfast together.
After this, Mama and Papa go to work and start working at their jobs.
Alice, Bob, Chris and Dave go to school, each studies in his/her class their specific subjects and after that they go home, each kid walks home alone when he finishes his classes.
At home, Alice and Bob wait for each other to start studying math together. Chris and Dave wait for each other to play football together.
After this, all the children wait for their parents to have dinner together. Everyone gets to sleep until next morning. Let your threads simulate the life of the Jones for 5 days, Monday-Friday. The threads will simulate activities by printing messages for the start and end of an activity, as well as messages when they are ready to start group activities.
An example of a part of the output could be:


Alice is ready to learn math with Bob
Dave is ready to play football with Chris
Chris is ready to play football with Dave
Chris plays football with Dave
Dave plays football with Chris
Dave is ready to eat dinner with family
Chris is ready to eat dinner with family
Bob is ready to learn math with Alice
Alice learns math with Bob
Bob learns math with Alice
Papa is ready to eat dinner with family
...
Benchmarking the Barrier Implementation
Create a benchmark program where you create NUMTHREADS threads. All threads will participate in a barrier. Each thread executes a loop repeating it REPEATS times where it is doing some dummy work in do_work function and then waits at the barrier. The do_work function can create a dummy workload by adding N float numbers.

```c
for (i = 0; i < REPEATS; i++) {
   do_work(thread_id);
   barrier_wait(&barrier);
}
```
Have one version of the benchmark program where you use pthread_barrier_t and one where you use my_barrier_t.

For different values of NUMTHREADS and different values of REPEATS, measure the runtimes of the program using pthread_barrier vs the program using my_barrier. For the same pair of NUMTHREADS and REPEATS values repeat the experiment several times to compute an average value. The difference between the average time with pthread_barrier and the average time with my_barrier will be due to the barrier implementation.
