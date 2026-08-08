//
// Created by os on 3/8/26.
//

#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/Thread.h"

// void* operator new (size_t size) {
//     return mem_alloc(size);
// }
//
// void operator delete (void* ptr) {
//     mem_free(ptr);
// }

Semaphore::Semaphore(unsigned int init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

int Semaphore::wait_n(unsigned n) {
    return sem_wait_n(this->myHandle, n);
}

int Semaphore::signal_n(unsigned n) {
    return sem_signal_n(this->myHandle, n);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread(void (*body)(void *), void *arg) :
    myHandle(nullptr), body(body), arg(arg) {}

Thread::Thread() :
    myHandle(nullptr), body(runWrapper), arg(this) {}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

void Console::putc(char c) {
    syscall_c::putc(c);
}

char Console::getc() {
    return syscall_c::getc();
}
