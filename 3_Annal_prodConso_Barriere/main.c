
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "prod_conso.h"
#include <assert.h>

int TAMPON_LENGTH = 10;
int NB_PROD_THREAD = 3;
int NB_CONSO_THREAD = 3;

msg tampon[10];

int  q_writer = 0;
int  q_reader = 0;

int  nb_messages = 0;

int idx_write = 0;
int idx_read = 0;

pthread_mutex_t mutex;
pthread_cond_t file_writer;
pthread_cond_t file_reader;

msg null_msg = { {0}, 0};


void *prod_routine(void *arg) {

    int thread_id = *(int *)arg;

    int i;

    for (i = 0; i< 5; i++){
        write(thread_id, thread_id + 10);   // thread_id is used as message also
        printf("finish write, go to sleep \n");
        usleep(1000);
        printf("wake up\n");
    }

    printf("========= PRODUCTEUR FINISH MISSION, EXIT \n");

    printf(" Etat du tampon : \n");

    print_tampon();
    
    return 0;
}

void *conso_routine(void *arg) {

    int thread_id = *(int *)arg;

    int i;

    for (i = 0; i< 5; i++){
        read(thread_id);
        usleep(1000);
    }

    printf("==========CONSOMMATEUR FINISH MISSION, EXIT \n");

    printf(" Etat du tampon : \n");

    print_tampon();
    
    return 0;
}

void print_tampon(){

    int i, j;

    for(i=0; i < 5; i++){
        for(j=0; j < 3; j++){
            printf(" %d , ", tampon[i].valeurs[j]);
        }
        printf("] \n");
        printf(" fill = %d \n, ", tampon[i].fill_nb);
        
    }
}

/*
State of programme - last update 26-11
All Conso and Producteurs finish and exit their routines
Messages are written to tampon, checked with msg = thread_id + 10
*/
int main(void) {

    //
    int i, j;

    for(i=0; i < 5; i++){
        for(j=0; j < 3; j++){
            tampon[i].valeurs[j] = -1;
        }
    }


    pthread_t t_conso[NB_CONSO_THREAD];
    pthread_t t_prod[NB_PROD_THREAD];


    pthread_mutex_init(&mutex, NULL);
  

    for (i = 0; i< (NB_PROD_THREAD); i++){
        
        pthread_create(&t_prod[i], NULL, &prod_routine, &i);
        pthread_create(&t_conso[i], NULL, &conso_routine, &i);
    
    }

    for (i = 0; i< (NB_PROD_THREAD); i++){
        
        pthread_join(t_prod[i], NULL);
        pthread_join(t_conso[i], NULL);
    
    }
    
    pthread_mutex_destroy(&mutex);

    


    return 0;
}