//
// Created by root on 10/30/24.
//
#include <stdio.h>
#include <pthread.h>

#include "my_routine.h"

int myInt = 0;
pthread_mutex_t mutex;
pthread_cond_t file_booth;

int main(void) {
    pthread_t t1;
    pthread_t t2;

    int *res1_p;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, &my_routine, NULL);
    pthread_create(&t2, NULL, &my_routine, NULL);// launch the thread
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}