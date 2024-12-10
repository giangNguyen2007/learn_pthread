//
// Created by root on 10/30/24.
//
//
// Created by root on 10/30/24.
//
//
// Created by root on 10/30/24.
//
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// add -pthread in compilation
int myInt = 0;
int TotalRessource = 200;
int fil = 0;
pthread_mutex_t mutex;
pthread_cond_t file_booth;

int getRessource() {

    int takeout = rand() % 100;

    pthread_mutex_lock(&mutex);

    while (TotalRessource < takeout) {
        fil++;
        pthread_cond_wait(&file_booth, &mutex);
        fil--;
    }
    TotalRessource -= takeout;
    pid_t myId = gettid();
    printf(" thread %d get %d ressource , new total = %d \n", myId, takeout, TotalRessource);
    pthread_mutex_unlock(&mutex);

    return takeout;
}

void *returnRessource(int takeout) {

    pthread_mutex_lock(&mutex);

    TotalRessource += takeout;
    pid_t myId = gettid();
    printf(" thread %d return %d ressource , new total = %d \n", myId, takeout, TotalRessource);

    // if there are thread waiting for ressource available, wake them up
    if (fil > 0) {
        pthread_cond_broadcast(&file_booth);
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}

void *routine() {
    while (1) {
        int takeout = getRessource();
        sleep(1);
        returnRessource(takeout);
    }

}

/*
Programme state : latest update 26-11
Running without error, 
Thread does not wait because Ressource never gets below 0, as 2 threads take and return ressources 
*/

int main(void) {
    pthread_t t1;
    pthread_t t2;
    

    int *res1_p;
    pthread_mutex_init(&mutex, NULL);
    pthread_create(&t1, NULL, &routine, NULL);
    
    pthread_create(&t2, NULL, &routine, NULL);// launch the thread
    pthread_setname_np(t1, "thread_1");
    pthread_setname_np(t2, "thread_2");
    
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&mutex);
    return 0;
}
