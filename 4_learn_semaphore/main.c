

// C program to demonstrate working of Semaphores
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t my_sem;

int THREAD_COUNT = 7;

void* take_from_sem()
{
    //wait
    sem_wait(&my_sem);
    printf("\nEntered  semaphore..\n");
 
}

void* add_to_sem()
{
    sleep(1);
    printf("\n Add one to sem...\n");
    sem_post(&my_sem);
}


int main()
{
    sem_init(&my_sem, 0, 0);
    pthread_t my_threads[THREAD_COUNT];

    pthread_create(&my_threads[0],NULL, &take_from_sem,NULL);
    pthread_create(&my_threads[1],NULL, &take_from_sem,NULL);
    pthread_create(&my_threads[2],NULL, &take_from_sem,NULL);

    pthread_create(&my_threads[3],NULL, &take_from_sem,NULL);
    pthread_create(&my_threads[4],NULL, &add_to_sem,NULL);
    //pthread_create(&my_threads[5],NULL, &add_to_sem,NULL);
    //pthread_create(&my_threads[6],NULL, &add_to_sem,NULL);
    
    int i;

    for (i = 0; i< (THREAD_COUNT-2); i++){
        
        pthread_join(my_threads[i], NULL);
    
    }


    sem_destroy(&my_sem);
    return 0;
}




