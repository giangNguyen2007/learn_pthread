#include "my_routine.h"
#include <stdio.h>

void *my_routine() {

    for (int i = 0; i < 1000000; ++i) {
        pthread_mutex_lock(&mutex);
        myInt++;
        pthread_mutex_unlock(&mutex);
    }
    printf("myInt = %d \n", myInt);

    return (void *)&myInt;

}