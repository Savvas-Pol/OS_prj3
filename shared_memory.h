#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H


void shared_memory_init();
void shared_memory_destroy();

void shared_memory_attach();
void shared_memory_detach();


void shared_memory_g_place(char command[20],int fakeid, int size, int duration);
void shared_memory_m_place(char command[20],int fakeid, int size, int duration);

void shared_memory_g_obtain(char command[20],int *fakeid, int *size, int * duration);
void shared_memory_m_obtain(char command[20],int *fakeid, int *size, int * duration);



#endif /* SHARED_MEMORY_H */

