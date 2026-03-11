//
// Created by os on 3/8/26.
//

#ifndef syscall_c
#define syscall_c

#include "../lib/hw.h"

void* mem_alloc (size_t size); // 0x01

int mem_free (void* ptr); // 0x02

size_t mem_get_free_space(); // 0x03

size_t mem_get_largest_free_block(); // 0x04

class myThread;
typedef myThread* thread_t;
int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg ); // 0x11

int thread_exit (); // 0x12

void thread_dispatch (); // 0x13

class mySemaphore;
typedef mySemaphore* sem_t;
int sem_open (sem_t* handle, unsigned init ); // 0x21

int sem_close (sem_t handle); //0x22

int sem_wait (sem_t id); //0x23

int sem_signal (sem_t id); // 0x24

int time_sleep(time_t time); // 0x31

const int EOF = -1;
char getc(); // 0x41

void putc (char c); // 0x42

#endif //syscall_c