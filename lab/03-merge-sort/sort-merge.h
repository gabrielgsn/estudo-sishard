#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct sort_args {
    int idxThread; 
    int nThreads; 
    int lineFiles; 
    int fdIn; 
    int fdOut; 
    pthread_mutex_t *mutex_file;
};
struct merge_args {
    int idxThread; 
    int nThreads; 
    int fdIn1,fdIn2;
    int fdOut;
};

void * sort(void *args);

void * merge(void *args);