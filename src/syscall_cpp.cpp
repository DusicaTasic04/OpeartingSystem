//
// Created by os on 3/8/26.
//

#include "../h/syscall_cpp.hpp"

void* operator new (size_t size) {
    return mem_alloc(size);
}

void operator delete (void* ptr) {
    mem_free(ptr);
}

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

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread(void (*body)(void *), void *arg) {
    thread_create(&this->myHandle, body, arg);
}

Thread::~Thread() {
    delete myHandle;
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

int Thread::sleep(time_t time) {
    return 0;
}

void Console::putc(char c) {
    syscall_c::putc(c);
}

char Console::getc() {
    return syscall_c::getc();
}
