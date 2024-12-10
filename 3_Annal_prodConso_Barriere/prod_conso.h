
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#ifndef MEM_INTERNALS_H
#define MEM_INTERNALS_H

extern int TAMPON_LENGTH;

extern int NB_PROD_THREAD;
extern int NB_CONSO_THREAD;

typedef struct _msg{
    int valeurs[3];
    int fill_nb;
} msg;

extern msg null_msg;

extern msg tampon[10];

extern int  q_writer;
extern int  q_reader;

extern int  count_writer_arrival;
extern int  count_reader_arrival;

extern int  nb_messages;

extern int idx_write;
extern int idx_read;

extern pthread_mutex_t mutex;
extern pthread_cond_t file_writer;
extern pthread_cond_t file_reader;

void read(int thread_id);
void write(int thread_id, int msg);



// ======== print color function
void print_red(char* str);

void blue();

void red();

void reset();

#endif