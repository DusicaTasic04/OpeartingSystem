//
// Created by os on 3/8/26.
//

#ifndef PROJECT_BASE_V1_2_SEMAPHORE_H
#define PROJECT_BASE_V1_2_SEMAPHORE_H

#include "List.h"
#include "Thread.h"

class mySemaphore {
public:
    mySemaphore(unsigned init);

    enum SemaphoreState {
        OPENED,
        CLOSED
    };

    int sem_close ();
    int sem_open (mySemaphore **handle, unsigned init);
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


#endif //PROJECT_BASE_V1_2_SEMAPHORE_H