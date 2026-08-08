//
// Created by os on 3/8/26.
//

#ifndef _Semaphore
#define _Semaphore

#include "List.h"
#include "Thread.h"

class mySemaphore {
public:
    explicit mySemaphore(unsigned init);
    static mySemaphore *createSemaphore(unsigned int init = 1);

    enum SemaphoreState {
        OPENED,
        CLOSED
    };

    int sem_close ();

    int sem_wait ();
    int sem_signal ();
    int sem_wait_n(unsigned n) volatile;

    void tryUnblock() volatile;

    int sem_signal_n(unsigned n) volatile;

    void block(unsigned n) volatile;

    uint64 getValue() const {
        return value;
    };

    SemaphoreState getState() const {
        return state;
    }

    void setState(const SemaphoreState s) {
        this->state = s;
    }

    void* operator new(size_t size) {
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }
    void* operator new[](size_t size) {
        return MemoryAllocator::getInstance()->mem_alloc(size);
    }

    void operator delete(void *ptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }
    void operator delete[](void *ptr) {
        MemoryAllocator::getInstance()->mem_free(ptr);
    }

protected:
    long value;
    SemaphoreState state;

private:
    List<myThread> blocked;
};


#endif //_Semaphore