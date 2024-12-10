

#include "main_exo_bulletin_vote.h"



void enter_urne(){

    pthread_mutex_lock(&mutex_urne);

    pid_t myId = gettid();


    
    while ((nb_entree_urne == 0 && q_waiting_avant_urne < 2) 
            || (nb_entree_urne == 3) 
            || (nb_left_urne %3 != 0) ) 
    {
        q_waiting_avant_urne++;
   
        printf(" thread %d sleep in queue , current queue length = %d \n", myId, q_waiting_avant_urne);
        pthread_cond_wait(&file_vant_urne, &mutex_urne);
        printf(" thread %d wake up \n", myId);

        q_waiting_avant_urne--;
    }

    blue();
    printf(" thread %d entrer dans l'urne \n", myId);
    nb_entree_urne++;
    printf(" current number in urne = %d \n", nb_entree_urne);

    
    if (nb_entree_urne < 3){
        // if fewer than 3 threads have entered urne, then wake up first thread in the queue
        pthread_cond_signal(&file_vant_urne);
    } else {
        printf(" enough 3 voters to enter into urne \n");
    }
    
    reset();

    pthread_mutex_unlock(&mutex_urne);

}


void leave_urne(){

    pthread_mutex_lock(&mutex_urne);

    pid_t myId = gettid();
    
    // wait until 3 threads have entered urne to leave 
    while (nb_entree_urne < 3) {
   
        printf(" thread %d attend dans l'urne \n", myId);
        pthread_cond_wait(&file_dans_urne, &mutex_urne);
        printf(" thread %d wake up \n", myId);
    
    }

    red();

    printf(" thread %d has voted and leaves urne \n", myId);
    nb_left_urne++;
    if (nb_left_urne % 3 == 0){
        printf(" >>>>>>> all 3 have left urne !!! \n");
        nb_entree_urne = 0;
        pthread_cond_broadcast(&file_vant_urne);
    } else {
        pthread_cond_signal(&file_dans_urne);
    }

    reset();

    pthread_mutex_unlock(&mutex_urne);

}