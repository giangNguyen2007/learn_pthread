//
// Created by root on 10/30/24.
//
//
// Created by root on 10/30/24.
//
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "main_exo_bulletin_vote.h"


const int NB_ISOLOIR_2 = 3;

int find_free_isoloir_with_bulletin(){
    int i = 0;
    while ( i < NB_ISOLOIR_2){

        // return i if booth free, and voting paper > 0
        if (!isoloir_occupe[i] && bulletins[i] > 0){ return i;}
        // if (!isoloir_occupe[i]){ return i;}
        i++;
    }
    return -1;
}


void *enter_voting_booth(int *i, int thread_id) {

    pthread_mutex_lock(&mutex);

    pid_t myId = gettid();
    printf(" thread %d obtain mutex lock \n", myId);
    //printf("thread %d has id = %d \n", myId, thread_id);
    while (find_free_isoloir_with_bulletin() == -1) {
        q_waiting_free_isoloir++;
        printf(" thread %d nb %d can not find free isoloir, go to waiting queue, queue length = %d \n", myId, thread_id, q_waiting_free_isoloir);
        pthread_cond_wait(&file_booth, &mutex);
        printf(" thread %d nb %d wake up \n", myId, thread_id);
        q_waiting_free_isoloir--;
    }

    *i = find_free_isoloir_with_bulletin();
    isoloir_occupe[*i] = true;
    visited_isoloir[thread_id] = *i;
    //printf("thread %d has id = %d \n", myId, thread_id);
    printf(" thread %d nb %d leave queue and enter isoloir number %d , queue length = %d \n", myId, thread_id,*i, q_waiting_free_isoloir);
    
    pthread_mutex_unlock(&mutex);

    return 0;
}

void *leave_voting_booth(int i, int thread_id) {

    pthread_mutex_lock(&mutex);

    pid_t myId = gettid();
    
    isoloir_occupe[i] = false;

    printf(" thread %d nb %d leave  isoloir number %d \n", myId , thread_id, i);

    if(bulletins[i] == 0 && q_filler == 1){
        printf(" isoloir nb %d empty, thread %d nb %d wake up filler \n", i, myId , thread_id);
        pthread_cond_signal(&file_filler);
    }

    // if queue not empty, wake up others
    if (q_waiting_free_isoloir > 0){
        pthread_cond_broadcast(&file_booth);
    }
    
    pthread_mutex_unlock(&mutex);

    return 0;
}




