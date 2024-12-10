#include <pthread.h>

extern int myInt;
extern pthread_mutex_t mutex;
extern pthread_cond_t file_booth;

void *my_routine();