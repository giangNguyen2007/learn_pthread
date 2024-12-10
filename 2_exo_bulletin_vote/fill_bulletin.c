

#include "main_exo_bulletin_vote.h"

const int NB_ISOLOIR_3 = 3;

// find isoloir sans bulltin
int find_isoloir_sans_bulletin(){
    int i = 0;
    while ( i < NB_ISOLOIR_3){
        // return i if booth free, and voting paper > 0
        if (bulletins[i] <= 0){ 
            printf("  bulletin dans isoloir nb %d est =  %d\n", i, bulletins[i]);
            return i;
        }
        i++;
    }
    return -1;
}


void trouver_isoloir_sans_bulletin(int *i, int thread_id) {

    pthread_mutex_lock(&mutex);

    pid_t myId = gettid();
    printf(" ========= filler thread %d obtain mutex lock \n", myId);
    //printf("thread %d has id = %d \n", myId, thread_id);
    while (find_isoloir_sans_bulletin() == -1) {
        printf(" ========== no more empty booth, filler go to sleep \n");
        q_filler++;
        pthread_cond_wait(&file_filler, &mutex);
        q_filler--;
        printf(" ======== filler %d wake up \n", myId);
    }

    *i = find_isoloir_sans_bulletin();
    printf(" isoloir sans bulletin est =  %d\n", *i);

    isoloir_occupe[*i] = true;
    //printf("thread %d has id = %d \n", myId, thread_id);
    printf(" =========== filler thread %d nb %d leave queue and enter isoloir number %d\n", myId, thread_id,*i);
    
    pthread_mutex_unlock(&mutex);

    return 0;
}