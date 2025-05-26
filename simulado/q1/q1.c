// Leia o PDF ou MD antes de iniciar este exercício!
// gcc q1.c -o q1 -pthread

// File-level overview:
// This program demonstrates thread synchronization using semaphores and a mutex
// to enforce a specific execution order for print statements across four threads.
// The desired order follows a dependency diagram:
// - 'A' must be printed before 'B', 'C', and 'D'.
// - 'B', 'C', and 'D' must be printed before 'E'.
// A mutex is used to ensure that the print statements themselves are atomic,
// preventing characters from different threads from interleaving.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

// Faça os includes necessários aqui
// #include ...

// SÓ PODE USAR A ESTRUTURA ABAIXO, NAO PODE USAR VARIAVEIS GLOBAIS
struct semaforos {
    // barrier: A semaphore used by thread1 to wait for thread2, thread3, and
    // thread4 to complete their tasks (printing 'B', 'C', 'D') before thread1
    // can print 'E'. It acts as a synchronization point.
    sem_t barrier;

    // sem_t2, sem_t3, sem_t4: Semaphores used by thread1 to signal thread2,
    // thread3, and thread4 respectively. This allows thread1 to print 'A'
    // first, and then unblock the other threads to print 'B', 'C', and 'D'.
    sem_t sem_t2, sem_t3, sem_t4;

    // mutex: A mutex to protect the critical sections where printf is called.
    // This ensures that character prints are atomic and not interleaved if multiple
    // threads were to print simultaneously (though in this specific logic,
    // prints are mostly sequenced by semaphores).
    pthread_mutex_t mutex;
    // Declare aqui os campos da sua sua estrutura para passar como argumento para as threads
};

// thread1: This function is the entry point for the first thread.
// - It first prints 'A'.
// - Then, it signals thread2, thread3, and thread4 to proceed by posting to their respective semaphores.
// - It then waits for three signals on the 'barrier' semaphore (one from each of thread2, thread3, thread4)
//   before printing 'E'. This ensures 'B', 'C', and 'D' are printed before 'E'.
// - The initial lock/unlock of the mutex at the beginning has no direct impact on synchronization here,
//   as no shared data is modified before printing 'A' and no other thread is competing for this mutex yet.
//   It might be a pattern placeholder or an artifact.
void *thread1(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    // This initial lock/unlock of the mutex does not protect any critical section
    // relevant to other threads at this point.
    pthread_mutex_lock(&sems->mutex);
    pthread_mutex_unlock(&sems->mutex);

    // Print 'A' - this is the first character to be printed.
    // The printf call is protected by a mutex to ensure atomicity, although
    // at this specific point, no other thread should be trying to print.
    pthread_mutex_lock(&sems->mutex);
    printf("A\n");
    pthread_mutex_unlock(&sems->mutex);

    // Signal thread2, thread3, and thread4 that they can proceed.
    sem_post(&sems->sem_t2);
    sem_post(&sems->sem_t3);
    sem_post(&sems->sem_t4);

    // Wait for thread2, thread3, and thread4 to complete their tasks.
    // Each of these threads will post to sem_barrier once done.
    sem_wait(&sems->barrier);
    sem_wait(&sems->barrier);
    sem_wait(&sems->barrier);

    // Print 'E' - this is the last character to be printed.
    // The printf call is protected by a mutex.
    pthread_mutex_lock(&sems->mutex);
    printf("E\n");
    pthread_mutex_unlock(&sems->mutex);

    return NULL;
}

// thread2: This function is the entry point for the second thread.
// - It waits on 'sem_t2' until signaled by thread1 (after 'A' is printed).
// - After being unblocked, it prints 'B'.
// - Then, it signals the 'barrier' semaphore to indicate its completion to thread1.
// - The printf call is protected by the mutex.
void *thread2(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    // Wait for thread1 to signal that it's okay to proceed.
    sem_wait(&sems->sem_t2);

    // Print 'B'. The printf call is protected by a mutex.
    pthread_mutex_lock(&sems->mutex);
    printf("B\n");
    pthread_mutex_unlock(&sems->mutex);

    // Signal thread1 that this thread has completed its task.
    sem_post(&sems->barrier);

    return NULL;
}

// thread3: This function is the entry point for the third thread.
// - It waits on 'sem_t3' until signaled by thread1 (after 'A' is printed).
// - After being unblocked, it prints 'C'.
// - Then, it signals the 'barrier' semaphore to indicate its completion to thread1.
// - The printf call is protected by the mutex.
void *thread3(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    // Wait for thread1 to signal that it's okay to proceed.
    sem_wait(&sems->sem_t3);

    // Print 'C'. The printf call is protected by a mutex.
    pthread_mutex_lock(&sems->mutex);
    printf("C\n");
    pthread_mutex_unlock(&sems->mutex);

    // Signal thread1 that this thread has completed its task.
    sem_post(&sems->barrier);

    return NULL;
}

// thread4: This function is the entry point for the fourth thread.
// - It waits on 'sem_t4' until signaled by thread1 (after 'A' is printed).
// - After being unblocked, it prints 'D'.
// - Then, it signals the 'barrier' semaphore to indicate its completion to thread1.
// - The printf call is protected by the mutex.
void *thread4(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    // Wait for thread1 to signal that it's okay to proceed.
    sem_wait(&sems->sem_t4);

    // Print 'D'. The printf call is protected by a mutex.
    pthread_mutex_lock(&sems->mutex);
    printf("D\n");
    pthread_mutex_unlock(&sems->mutex);

    // Signal thread1 that this thread has completed its task.
    sem_post(&sems->barrier);

    return NULL;
}

// main: The main function of the program.
// - Declares pthread_t variables for the four threads (t1, t2, t3, t4) and
//   an instance of the 'semaforos' struct ('sems') to hold semaphores and the mutex.
// - Initializes all semaphores:
//   - 'barrier' is initialized to 0, so thread1 will block when it waits on this
//     semaphore until it's posted by threads 2, 3, and 4.
//   - 'sem_t2', 'sem_t3', 'sem_t4' are initialized to 0, so threads 2, 3, and 4
//     will block initially until posted by thread1.
// - Initializes the 'mutex'.
// - Creates the four threads (thread1, thread2, thread3, thread4), passing the
//   'sems' struct (containing the semaphores and mutex) as an argument to each.
// - Waits for all four threads to complete using pthread_join. This ensures that
//   the main function does not exit before the threads finish their execution.
// - Destroys all semaphores and the mutex to free the resources they were using.
// - The program structure, through the use of these semaphores, ensures that
//   the print dependencies (A -> B, A -> C, A -> D) and (B,C,D -> E) are met.
int main(int argc, char *argv[]) {

    // Declare thread identifiers
    pthread_t t1, t2, t3, t4;
    // Declare the structure to hold semaphores and mutex
    struct semaforos sems;

    // Initialize the semaphores:
    // barrier: starts at 0, so thread1 waits until other threads signal.
    sem_init(&sems.barrier, 0, 0);
    // sem_t2, sem_t3, sem_t4: start at 0, so threads 2, 3, 4 wait for thread1.
    sem_init(&sems.sem_t2, 0, 0);
    sem_init(&sems.sem_t3, 0, 0);
    sem_init(&sems.sem_t4, 0, 0);

    // Initialize the mutex
    pthread_mutex_init(&sems.mutex, NULL);

    // Create the four threads, passing the 'sems' struct as an argument.
    pthread_create(&t1, NULL, thread1, (void *)&sems);
    pthread_create(&t2, NULL, thread2, (void *)&sems);
    pthread_create(&t3, NULL, thread3, (void *)&sems);
    pthread_create(&t4, NULL, thread4, (void *)&sems);


    // Wait for all threads to complete their execution.
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    // Destroy the semaphores and mutex to free allocated resources.
    sem_destroy(&sems.barrier);
    sem_destroy(&sems.sem_t2);
    sem_destroy(&sems.sem_t3);
    sem_destroy(&sems.sem_t4);
    pthread_mutex_destroy(&sems.mutex);
    
    
    return 0;
}
