
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#ifndef MEM_INTERNALS_H
#define MEM_INTERNALS_H

extern int TAMPON_LENGTH;

// typedef struct _msg{
//     int valeurs[3];
// } msg;

extern int tampon[10];

extern int  q_writer;
extern int  q_reader;

extern int  nb_messages;

extern int idx_write;
extern int idx_read;

extern pthread_mutex_t mutex;
extern pthread_cond_t file_writer;
extern pthread_cond_t file_reader;

void read();
void write(int msg);



// ======== print color function
void print_red(char* str);

void blue();

void red();

void reset();

#endif