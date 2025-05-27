// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
// para compilar use:

// gcc q1.c -o q1 -pthread

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

struct thread_args {
    sem_t sem1;
    sem_t sem2;
    sem_t sem3;
    sem_t sem4;
};

void *thread1(void *_arg) {
    struct thread_args *sems = (struct thread_args *)_arg;

    printf("A\n");

    sem_post(&sems->sem1);
    sem_wait(&sems->sem4);

    printf("C\n");

    sem_post(&sems->sem1);
    sem_post(&sems->sem1);
    sem_wait(&sems->sem3);

    printf("E\n");

    return NULL;
}

void *thread2(void *_arg) {
    struct thread_args *sems = (struct thread_args *)_arg;

    sem_wait(&sems->sem1);
    sem_wait(&sems->sem3);    

    printf("F\n");

    return NULL;
}

void *thread3(void *_arg) {
    struct thread_args *sems = (struct thread_args *)_arg;

    sem_wait(&sems->sem1);
    sem_wait(&sems->sem4);

    printf("D\n");

    sem_post(&sems->sem3);
    sem_post(&sems->sem3);
    sem_post(&sems->sem3);

    return NULL;
}

void *thread4(void *_arg) {
    struct thread_args *sems = (struct thread_args *)_arg;

    printf("B\n");
    
    sem_post(&sems->sem4);
    sem_post(&sems->sem4);

    sem_wait(&sems->sem1);
    sem_wait(&sems->sem3);

    printf("G\n");

    return NULL;
}

int main(int argc, char *argv[]) {

    pthread_t t1, t2, t3, t4;

    struct thread_args sems;

    sem_init(&sems.sem1, 0, 0);
    sem_init(&sems.sem2, 0, 0);
    sem_init(&sems.sem3, 0, 0);
    sem_init(&sems.sem4, 0, 0);

    pthread_create(&t1, NULL, thread1, (void *)&sems);
    pthread_create(&t2, NULL, thread2, (void *)&sems);
    pthread_create(&t3, NULL, thread3, (void *)&sems);
    pthread_create(&t4, NULL, thread4, (void *)&sems);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    sem_destroy(&sems.sem1);
    sem_destroy(&sems.sem2);
    sem_destroy(&sems.sem3);
    sem_destroy(&sems.sem4);

    return 0;
}