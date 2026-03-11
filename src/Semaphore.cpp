//
// Created by os on 3/8/26.
//

#include "../h/Semaphore.h"

mySemaphore::mySemaphore(unsigned init) {
    this->value = init;
    this->state = OPENED;
}

int mySemaphore::sem_close() {
    if (this->state == CLOSED) {
        return -1;
    }
    this->state = CLOSED;

    while (blocked.getFirst()) {
        myThread *t = blocked.removeFirst();
        t->setBlocked(false);
        Scheduler::getInstance().put(t);
    }

    return 0;
}

mySemaphore *mySemaphore::createSemaphore(unsigned int init) {
    return new mySemaphore(init);
}

int mySemaphore::sem_wait() {
    if (this->state == CLOSED) {
        return -1;
    }

    if (--value < 0) {
        myThread::running->setBlocked(true); // blokiram trenutnu nit
        blocked.addLast(myThread::running); // vracam je na kraj
        myThread::yield(); // promena konteksta
    }

    return 0;
}

int mySemaphore::sem_signal() {
    if (this->state == CLOSED) {
        return -1;
    }

    if (++value <= 0) {
        myThread *deblocked = blocked.removeFirst();
        //if (!deblocked) return -1;
        deblocked->setBlocked(false);
        Scheduler::getInstance().put(deblocked);
    }

    return 0;
}

// void signalWait (Semaphore* s, Semaphore* w) {
//     lock();
//     if (s && s->value++<0) s->deblock();
//     if (w && --w->value<0) w->block();
//     unlock();
// }