
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "prod_conso.h"

void write(int msg){

    pthread_mutex_lock(&mutex);

    pid_t myId = gettid();
    printf(" >>>>>>>>> PRODUCTEUR : thread %d obtain mutex lock \n", myId);
    //printf("thread %d has id = %d \n", myId, thread_id);
    while (nb_messages == TAMPON_LENGTH) {
        blue();
        q_writer++;
        printf(" <<<<<<<<< PRODUCTEUR : tampon full, thread %d  unlock mutext and wait in queue , q length = %d \n", myId, q_writer);
        reset();
        pthread_cond_wait(&file_writer, &mutex);

        blue();
        printf(" PRODUCTEUR thread %d wake up \n", myId);
        reset();

        q_writer--;
    }

    // if tampon not full, write to tampon

    tampon[idx_write] = msg;
    blue();
    printf("PRODUCTEUR: thread %d write message %d at indice %d \n", myId, msg, idx_write);
    reset();
    nb_messages++;

    idx_write = (idx_write + 1)%TAMPON_LENGTH;  
    
    //if there are consommateur waiting for message, wake them up
    if (q_reader > 0){
        printf("PRODUCTEUR: thread %d wake up fil conso \n", myId);
        pthread_cond_broadcast(&file_reader);
    }
    
    printf("<<<<<<<<< PRODUCTEUR: thread %d unlock mutex\n", myId);
    pthread_mutex_unlock(&mutex);

    return 0;

}

void read(){

    pthread_mutex_lock(&mutex);

    pid_t myId = gettid();
    printf(" >>>>>>> CONSOMATEUR : thread %d obtain mutex lock \n", myId);

    // wait if there is no message
    while (nb_messages == 0) {
        q_reader++;
        printf(" <<<<<<<< CONSOMMATEUR : tampon vide, thread %d unlock mutext &  wait in queue , q length = %d \n", myId, q_reader);
        pthread_cond_wait(&file_reader, &mutex);
        printf(" CONSOMMATEUR thread %d wake up \n", myId);
        q_reader--;
    }

    // if tampon not empty, read

    printf("CONSOMMATEUR: thread %d read message %d at indice %d \n", myId, tampon[idx_read], idx_read);
    tampon[idx_read] = NULL;
    nb_messages--;
    
    idx_read = (idx_read + 1)%TAMPON_LENGTH; 
    
    // if there are producteur waiting for message, wake them up
    if (q_writer > 0){
        pthread_cond_broadcast(&file_writer);
    }
    
    printf("<<<<<<<<< CONSOMATEUR: thread %d unlock mutex\n", myId);
    pthread_mutex_unlock(&mutex);

    return 0;

}