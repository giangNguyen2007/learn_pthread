
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "prod_conso.h"
#include <assert.h>

int TAMPON_LENGTH = 10;

int tampon[10] = {-1};

int  q_writer = 0;
int  q_reader = 0;

int  nb_messages = 0;

int idx_write = 0;
int idx_read = 0;

pthread_mutex_t mutex;
pthread_cond_t file_writer;
pthread_cond_t file_reader;


void *prod_routine(void *arg) {

    int thread_id = *(int *)arg;

    int i;

    for (i = 0; i< 10; i++){
        write(thread_id);
        printf("finish write, go to sleep \n");
        usleep(1000);
        printf("wake up\n");
    }

    printf("========= PRODUCTEUR FINISH MISSION, EXIT \n");
    
    return 0;
}

void *conso_routine(void *arg) {

    int thread_id = *(int *)arg;

    int i;

    for (i = 0; i< 10; i++){
        read();
        usleep(1000);
    }

    printf("==========CONSOMMATEUR FINISH MISSION, EXIT \n");
    
    return 0;
}


int main(void) {

    int i;

    int nb_thread = 2;
    pthread_t t_conso[nb_thread];
    pthread_t t_prod[nb_thread];


    pthread_mutex_init(&mutex, NULL);
  
    // int filler_thread_id = 100;
    // pthread_create(&filler_thread, NULL, &filler_thread_routine, &filler_thread_id);

    for (i = 0; i< (nb_thread); i++){
        
        pthread_create(&t_prod[i], NULL, &prod_routine, &i);
        pthread_create(&t_conso[i], NULL, &conso_routine, &i);
    
    }

    for (i = 0; i< (nb_thread); i++){
        
        pthread_join(t_prod[i], NULL);
        pthread_join(&t_conso[i], NULL);
    
    }
    
    pthread_mutex_destroy(&mutex);


    return 0;
}