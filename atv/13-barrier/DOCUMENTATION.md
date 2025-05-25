# Documentation for `atv/13-barrier` - Thread Synchronization with Barriers

This directory contains a C program (`barrier.c`) intended to demonstrate the use of barriers for synchronizing multiple threads. The user is expected to complete the `TODO` sections to implement the barrier logic.

## Concepts

### Barrier Synchronization
A barrier is a synchronization primitive that ensures that multiple threads wait at a particular point (the "barrier") until all participating threads have reached it. Once all threads have arrived at the barrier, they are all allowed to proceed.

Barriers are useful in parallel programming when a computation is divided into phases, and all threads must complete a phase before any thread can move on to the next phase.

### POSIX Barriers (`pthread_barrier_t`)
The POSIX threads library provides a direct implementation for barriers:
*   **`pthread_barrier_t`**: A type representing a barrier object.
*   **`pthread_barrier_init(barrier, attr, count)`**: Initializes a barrier object.
    *   `barrier`: A pointer to a `pthread_barrier_t` object.
    *   `attr`: Barrier attributes (usually `NULL` for default).
    *   `count`: The number of threads that must call `pthread_barrier_wait()` before any of them will proceed.
*   **`pthread_barrier_wait(barrier)`**: Marks that the calling thread has reached the barrier.
    *   The thread will block until `count` threads (specified during `pthread_barrier_init`) have called `pthread_barrier_wait()`.
    *   Once `count` threads are waiting, all of them unblock and proceed.
    *   One arbitrarily chosen thread will return `PTHREAD_BARRIER_SERIAL_THREAD` from `pthread_barrier_wait()`, while others return 0. This can be used to have one thread perform a special action after the barrier.
*   **`pthread_barrier_destroy(barrier)`**: Destroys a barrier object, freeing any resources it might hold.

### Implementing Barriers Manually (Conceptual)
If `pthread_barrier_t` is not used or available, a barrier can be conceptually implemented using:
*   A **mutex** (`pthread_mutex_t`): To protect shared variables (like a counter).
*   A **condition variable** (`pthread_cond_t`): To allow threads to wait efficiently until a condition (e.g., all threads have arrived) is met.
*   A **counter**: To keep track of how many threads have arrived at the barrier.

The logic would be:
1.  Initialize mutex, condition variable, and counter (set to 0). Set the target count of threads.
2.  When a thread reaches the barrier:
    a.  Lock the mutex.
    b.  Increment the counter.
    c.  If counter < target count:
        i.  Wait on the condition variable (this atomically unlocks the mutex and puts the thread to sleep).
    d.  Else (counter == target count, i.e., this is the last thread):
        i.  Signal/broadcast on the condition variable to wake up all waiting threads.
    e.  Unlock the mutex.

## `barrier.c` Program Explained

The `barrier.c` file provides a skeleton for a program where multiple threads perform some work, synchronize at a barrier, and then perform more work.

### Provided Code Structure:

*   **`thread_barreira(void *arg)` function**:
    *   This is the function that each thread will execute.
    *   It prints "Antes da barreira".
    *   Contains a `// TODO: implemente a barreira aqui` comment where the barrier logic should be placed.
    *   It prints "Depois da barreira".

*   **`main()` function**:
    *   Contains `// TODO: crie 4 threads rodando o código acima` where 4 threads should be created, each running `thread_barreira`.
    *   Contains `// TODO: espere por elas` where the main thread should wait for all 4 worker threads to complete.

### Task: Completing the `TODO`s

The user needs to implement the following:

1.  **In `main()`**:
    *   Declare a `pthread_barrier_t` variable.
    *   Initialize it using `pthread_barrier_init()` with a `count` of 4 (since 4 worker threads will be created).
    *   Create an array of `pthread_t` to store thread IDs.
    *   Loop 4 times to create the threads using `pthread_create()`. Each thread should run `thread_barreira`. You might need to pass the barrier object to `thread_barreira` if it's not global (passing as argument is cleaner).
    *   Loop 4 times to wait for each thread to complete using `pthread_join()`.
    *   Destroy the barrier using `pthread_barrier_destroy()` before exiting `main`.

2.  **In `thread_barreira(void *arg)`**:
    *   The `arg` can be used to pass a pointer to the barrier object if it's not global.
    *   At the `// TODO: implemente a barreira aqui` location, call `pthread_barrier_wait()` with the barrier object.

## Example of Completed Code (Conceptual)

Here's how the `TODO` sections could be filled using `pthread_barrier_t`:

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // For exit()

// Global barrier (or pass as argument)
pthread_barrier_t my_barrier;

void *thread_barreira(void *arg) {
    // Cast arg if barrier is passed as argument
    // pthread_barrier_t *barrier_ptr = (pthread_barrier_t *)arg;

    // NÃO altere este print, ele simula uma tarefa
    printf("Thread %lu: Antes da barreira
", pthread_self());

    // Implemente a barreira aqui
    int rc = pthread_barrier_wait(&my_barrier); // Use barrier_ptr if passed as arg
    if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD) {
        printf("Thread %lu: Could not wait on barrier
", pthread_self());
        exit(-1);
    }

    // NÃO altere este print, ele simula uma tarefa
    printf("Thread %lu: Depois da barreira
", pthread_self());

    return NULL;
}

int main() {
    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];

    // Initialize the barrier for NUM_THREADS threads
    if (pthread_barrier_init(&my_barrier, NULL, NUM_THREADS)) {
        printf("Could not create a barrier
");
        return -1;
    }

    // Crie 4 threads rodando o código acima
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, thread_barreira, NULL)) { // Pass &my_barrier as last arg if needed
            printf("Could not create thread %d
", i);
            return -1;
        }
    }

    // Espere por elas
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            printf("Could not join thread %d
", i);
            return -1;
        }
    }

    // Destroy the barrier
    if (pthread_barrier_destroy(&my_barrier)) {
        printf("Could not destroy the barrier
");
        return -1;
    }

    printf("All threads completed successfully.
");
    return 0;
}
```
*(Note: The example adds thread IDs to prints for clarity and includes basic error checking, which is good practice.)*

## Compilation and Execution

### Compilation
Once the `TODO`s are completed, compile the program, linking with the pthreads library:
```bash
gcc barrier.c -o barrier -pthread -Wall -Og -g
```

### Expected Output
When run, the program should first print "Antes da barreira" four times (order may vary among threads). Then, after all threads have hit the barrier, it should print "Depois da barreira" four times (order may vary).
Example (thread IDs will differ):
```
Thread 140...: Antes da barreira
Thread 139...: Antes da barreira
Thread 141...: Antes da barreira
Thread 138...: Antes da barreira
Thread 140...: Depois da barreira
Thread 138...: Depois da barreira
Thread 141...: Depois da barreira
Thread 139...: Depois da barreira
All threads completed successfully.
```

This exercise is designed to teach the use of barriers for thread synchronization, a fundamental concept in multi-threaded programming.
```
