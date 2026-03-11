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

    uint64 getValue() const {
        return value;
    };

    SemaphoreState getState() const {
        return state;
    }

    void setState(const SemaphoreState s) {
        this->state = s;
    }

protected:
    uint64 value;
    SemaphoreState state;

private:
    List<myThread> blocked;
};


#endif //_Semaphore