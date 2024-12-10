
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "prod_conso.h"

void write(int thread_id, int msg){

    pthread_mutex_lock(&mutex);

    pid_t myId = gettid();
    printf(" >>>>>>>>> PRODUCTEUR no %d: thread %d obtain mutex lock \n", thread_id, myId);

    //if tampon is full, wait for consummers to read
    while (nb_messages == TAMPON_LENGTH) {
        blue();
        q_writer++;
        printf(" <<<<<<<<< PRODUCTEUR no %d: tampon full, thread %d  unlock mutext and wait in queue , q length = %d \n", thread_id,myId, q_writer);
        reset();
        pthread_cond_wait(&file_writer, &mutex);

        blue();
        printf(" PRODUCTEUR no %d: thread %d wake up \n", thread_id, myId);
        reset();

        q_writer--;
    }

    // write to case 

    tampon[idx_write].valeurs[thread_id] = msg;
    tampon[idx_write].fill_nb++;
    printf(" PRODUCTEUR %d : write message to case nb %d of at tampon index of %d, fill_value =  \n", thread_id, thread_id, idx_write, tampon[idx_write].fill_nb);

    int old_idx_write = idx_write;
    // wait for others writer to complete the message
    while (tampon[old_idx_write].fill_nb < NB_PROD_THREAD) {
        blue();
        q_writer++;
        printf(" <<<<<<<<< PRODUCTEUR : wait for other message to finish,  unlock mutext and wait in queue , q length = %d \n", myId, q_writer);
        reset();
        pthread_cond_wait(&file_writer, &mutex);

        blue();
        printf(" PRODUCTEUR thread %d wake up \n", myId);
        reset();
        q_writer--;
    }

    

    //if there are producteurs waiting until reaching barrier, wake them up
    if (q_writer == NB_PROD_THREAD - 1){
        printf("PRODUCTEUR: thread %d wake up fil writer \n", myId);
        pthread_cond_broadcast(&file_writer);
    }

    // when all producteur est sorti du fil, mettre le message disponible au lecteurs
    if (q_writer == 0){
        nb_messages++;
        idx_write = (idx_write + 1)%TAMPON_LENGTH;
    }
    
    //if there are consommateur waiting for message, wake them up
    if (q_reader > 0){
        printf("PRODUCTEUR: thread %d wake up fil reader \n", myId);
        pthread_cond_broadcast(&file_reader);
    }
    
    printf("<<<<<<<<< PRODUCTEUR nb %d : thread %d unlock mutex\n", thread_id, myId);
    pthread_mutex_unlock(&mutex);

    return 0;

}

void read(int thread_id){

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
    tampon[idx_read].fill_nb--;


    int old_idx_read = idx_read;
    // wait for others reader to complete reading the message
    while (tampon[old_idx_read].fill_nb > 0) {
        blue();
        q_reader++;
        printf(" <<<<<<<< CONSOMMATEUR :thread %d wait for other reader thread to finish reading message, go to sleep \n", myId, q_reader);
        pthread_cond_wait(&file_reader, &mutex);

        blue();
        printf(" CONSOMMATEUR thread %d wake up \n", myId);
        reset();
        q_reader--;
    }


    //if there are conso thread waiting until reaching barrier, wake them up
    if (q_reader == NB_CONSO_THREAD - 1){
        printf("CONSOMATEUR: thread %d wake up fil writer \n", myId);
        pthread_cond_broadcast(&file_reader);
    }

    if (q_reader == 0){
        //tampon[idx_read] = null_msg;
        nb_messages--;
        idx_read = (idx_read + 1)%TAMPON_LENGTH; 
    }
    
    // if there are producteur waiting for message, wake them up
    if (q_writer > 0){
        pthread_cond_broadcast(&file_writer);
    }
    
    printf("<<<<<<<<< CONSOMATEUR: thread %d unlock mutex\n", myId);
    pthread_mutex_unlock(&mutex);

    return 0;

}