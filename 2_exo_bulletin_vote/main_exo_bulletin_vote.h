
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#ifndef MEM_INTERNALS_H
#define MEM_INTERNALS_H

#define MY_NB_ISOLOIR 3;

// add -pthread in compilation
extern bool isoloir_occupe[];

// nb of bulletin par isoloir - initial = 7
extern int bulletins[];
extern int visited_isoloir[];

extern int thread_id_counter ;

// tracking nb of thread in waiting 
extern int  q_waiting_free_isoloir;
extern int  q_filler;


extern int  q_waiting_avant_urne;

extern int  nb_entree_urne;
extern int  nb_left_urne;


extern pthread_mutex_t mutex;
extern pthread_cond_t file_booth;
extern pthread_cond_t file_filler;

extern pthread_mutex_t mutex_urne;
extern pthread_cond_t file_vant_urne;
extern pthread_cond_t file_dans_urne;


void trouver_isoloir_sans_bulletin(int *i, int thread_id);

// void *enter_voting_booth(int *i, int thread_id);

// void *leave_voting_booth(int i, int thread_id);


void enter_urne();
void leave_urne();

// ======== print color function
void print_red(char* str);

void blue();

void red();

void reset();

#endif