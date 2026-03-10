//
// Created by os on 3/8/26.
//

#include "../h/syscall_c.hpp"
#include "../h/Riscv.h"

void* mem_alloc (size_t size) {
    size *= MEM_BLOCK_SIZE; // Zato sto moj mem_alloc vec ubacuje kao broj bajtova
    __asm__ volatile("mv a1, %0" :: "r"(size)); // prosledim parametre

    __asm__ volatile ("mv a0, %0" :: "r"(MEM_ALLOC)); // u a0 premestim kod
    __asm__ volatile ("ecall"); // skok u privilegovani rezim

    // povratak iz privilegovanog rezima:
    void* returnValue;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

int mem_free(void* ptr) {
    __asm__ volatile ("mv a1, %0" :: "r" (ptr));

    __asm__ volatile ("mv a0, %0" :: "r" (MEM_FREE)); // u a0 premestim kod
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

size_t mem_get_free_space() {
    __asm__ volatile ("mv a0, %0" :: "r" (MEM_GET_FREE_SPACE));
    __asm__ volatile ("ecall");

    size_t returnValue;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

size_t mem_get_largest_free_block() {
    __asm__ volatile ("mv a0, %0" :: "r" (MEM_GET_LARGEST_FREE_BLOCK));
    __asm__ volatile ("ecall");

    size_t returnValue;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg ) {
    __asm__ volatile("mv a7, %0" :: "r" (arg));
    __asm__ volatile("mv a2, %0" :: "r" (start_routine));
    __asm__ volatile("mv a1, %0" :: "r" (handle));

    __asm__ volatile ("mv a0, %0" :: "r" (THREAD_CREATE));
    __asm__ volatile("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

int thread_exit () {
    __asm__ volatile ("mv a0, %0" :: "r" (THREAD_EXIT));
    __asm__ volatile("ecall");

    size_t returnValue;
    __asm__ volatile ("mv %0, a0":"=r" (returnValue));
    return returnValue;
}

void thread_dispatch () {
    __asm__ volatile ("mv a0, %0" :: "r" (THREAD_DISPATCH));
    __asm__ volatile("ecall");
}

int sem_open (sem_t* handle, unsigned init ) {
    __asm__ volatile("mv a2, %0" :: "r" (init));
    __asm__ volatile("mv a1, %0" :: "r" (handle));

    __asm__ volatile ("mv a0, %0" :: "r" (SEM_OPEN));

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

int sem_close (sem_t handle) {
    __asm__ volatile("mv a1, %0" :: "r" (handle));

    __asm__ volatile ("mv a0, %0" :: "r"(SEM_CLOSE));
    __asm__ volatile("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int sem_wait (sem_t id) {
    __asm__ volatile("mv a1, %0" :: "r" (id));

    __asm__ volatile ("mv a0, %0" :: "r"(SEM_WAIT));
    __asm__ volatile("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int sem_signal (sem_t id) {
    __asm__ volatile("mv a1, %0" :: "r" (id));

    __asm__ volatile ("mv a0, %0" :: "r"(SEM_SIGNAL));
    __asm__ volatile("ecall");

    int returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int time_sleep(time_t time) {
    __asm__ volatile ("mv a1, %0" :: "r" (time));

    __asm__ volatile ("mv a0, %0" :: "r"(TIME_SLEEP));
    __asm__ volatile ("ecall");

    int returnValue;
    __asm__ volatile("mv %0, a0" : "=r" (returnValue));
    return returnValue;
}

char getc() {
    __asm__ volatile ("mv a0, %0" :: "r"(GETC));
    __asm__ volatile ("ecall");

    char returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

void putc(char c) {
    __asm__ volatile ("mv a1, %0" : : "r" (c));
    __asm__ volatile ("mv a0, %0" :: "r"(PUTC));
    __asm__ volatile ("ecall");
}

