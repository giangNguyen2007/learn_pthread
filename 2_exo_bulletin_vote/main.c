
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#include "main_exo_bulletin_vote.h"
#include <assert.h>

const int NB_ISOLOIR = 3;

// add -pthread in compilation
bool isoloir_occupe[2] = {false};

// nb of bulletin par isoloir - initial = 7
int bulletins[2] = {3};
int visited_isoloir[2] = {-1};

int thread_id_counter = 0;

// tracking nb of thread in waiting 
int q_waiting_free_isoloir = 0;
int q_filler = 0;

int  q_waiting_avant_urne = 0;
int  nb_entree_urne = 0;
int  nb_left_urne = 0;

pthread_mutex_t mutex;
pthread_cond_t file_booth;
pthread_cond_t file_filler;


pthread_mutex_t mutex_urne;
pthread_cond_t file_vant_urne;
pthread_cond_t file_dans_urne;



void *normal_thread_routine(void *arg) {

    pid_t myId = gettid();
    // int thread_id = thread_id_counter++;
    int thread_id = *(int *)arg;
    //int id = *(int *)arg;
    printf(" thread %d get id number = nb %d \n", myId, thread_id);

    int isoloir_number;
    enter_voting_booth(&isoloir_number, thread_id);

    bulletins[isoloir_number]--;
    printf(" thread %d id = nb %d , pris bulletin \n", myId, thread_id);
    printf(" nb bulletin restant en isoloir nb %d = %d \n", isoloir_number, bulletins[isoloir_number]);
    assert(bulletins[isoloir_number] >= 0);

    sleep(1);
    leave_voting_booth(isoloir_number, thread_id);
    
    printf(" thread %d nb %d terminated \n", myId, thread_id);
    
    return 0;
}

void *filler_thread_routine(void *arg) {

    pid_t myId = gettid();
    // int thread_id = thread_id_counter++;
    int thread_id = *(int *)arg;
    //int id = *(int *)arg;
    printf(" thread %d get id number = nb %d \n", myId, thread_id);

    int isoloir_number;

    while(true){

        trouver_isoloir_sans_bulletin(&isoloir_number, thread_id);
        bulletins[isoloir_number]+= 2;
        red();
        printf(" ======= filler_thread %d nb %d fill isoloir nb %d with 3 bulletins \n", myId, thread_id, isoloir_number);
        printf(" ======= nb of bulletins in isoloir nb %d = %d \n", isoloir_number, bulletins[isoloir_number]);
        reset();
        leave_voting_booth(isoloir_number, thread_id);

    }
    
    printf(" thread %d nb %d terminated \n", myId, thread_id);
    
    return 0;
}

int urne_routine() {

    pid_t myId = gettid();
    // int thread_id = thread_id_counter++;
    // int thread_id = *(int *)arg;
    //int id = *(int *)arg;
    //printf(" thread %d get id number = nb %d \n", myId, thread_id);
    enter_urne();
    usleep(1000);
    leave_urne();
    
    printf(" thread %d nb %d terminated \n", myId);
    
    return 0;
}


int main(void) {

    int i;

    for (i = 0; i< 3; i++){
        //printf("thread number %d started \n", i);
        bulletins[i] = 2;
        
    }

    int nb_thread = 10;
    pthread_t t[nb_thread - 1];

    pthread_t filler_thread;

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_urne, NULL);

    

  
    // int filler_thread_id = 100;
    // pthread_create(&filler_thread, NULL, &filler_thread_routine, &filler_thread_id);

   
    for (i = 0; i< (nb_thread - 1); i++){
        //printf("thread number %d started \n", i);
        // pthread_create(&t[i], NULL, &normal_thread_routine, &i);
        pthread_create(&t[i], NULL, &urne_routine, NULL);
        
    }

    for (i = 0; i< (nb_thread - 1); i++){
        pthread_join(t[i], NULL);
        //printf("thread number %d terminated \n", i);
    }

    // pthread_join(filler_thread, NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_urne);

    printf(" ALL THREAD TERMINATED \n");

 
    return 0;
}