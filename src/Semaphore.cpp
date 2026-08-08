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
    // projekat iz 2025. godine

    // if (this->state == CLOSED) {
    //     return -1;
    // }
    //
    // myThread::running->waitingFor = 1;
    //
    // if (--value < 0) {
    //
    //     blocked.addLast(myThread::running); // vracam je na kraj
    //     myThread::running->setBlocked(true); // blokiram trenutnu nit
    //     myThread::yield(); // promena konteksta
    // }
    //
    // return 0;

    return sem_wait_n(1);
}

int mySemaphore::sem_signal() {
    // projekat iz 2025. godine

    // if (this->state == CLOSED) {
    //     return -1;
    // }
    //
    // if (++value <= 0) {
    //     myThread *deblocked = blocked.removeFirst();
    //     //if (!deblocked) return -1;
    //     deblocked->setBlocked(false);
    //     Scheduler::getInstance().put(deblocked);
    // }
    //
    // return 0;

    return sem_signal_n(1);
}

int mySemaphore::sem_wait_n(unsigned n) volatile {
    if (this->state == CLOSED) {
        return -1;
    }

    if (n == 0) return 0;

    if (blocked.getFirst() || value < (int)n) {
        block(n);
        if (myThread::running->isBlocked()) {
            myThread::running->setBlocked(false);
            return -1;
        }
        return 0;
    }

    value -= (int)n;
    return 0;
}

void mySemaphore::tryUnblock() volatile {
    myThread* unblockedThread = blocked.removeFirst();
    if (!unblockedThread) return;
    value -= unblockedThread->waitingFor;
    unblockedThread->waitingFor = 0;
    unblockedThread->setBlocked(false);
    Scheduler::getInstance().put(unblockedThread);
}

int mySemaphore::sem_signal_n(unsigned n) volatile {

    // if (n == 0) return 0;
    // val += (int)n;
    // while (blockedThreadsQueue.peekFirst() &&
    //        val >= (int)blockedThreadsQueue.peekFirst()->getSemaphoreRequest()) {
    //     tryUnblock();
    //        }
    // return 0;


    if (this->state == CLOSED) {
        return -1;
    }

    if (n == 0) return 0;

    value += (int)n;

    while (blocked.getFirst() && value >= blocked.getFirst()->waitingFor) {
        tryUnblock();
    }

    return 0;
    // while (true) {
    //     myThread* t = blocked.getFirst();
    //     if (!t) break;
    //
    //     if (value >= t->waitingFor) {
    //         blocked.removeFirst();
    //
    //         value -= t->waitingFor;
    //
    //         t->setBlocked(false);
    //         Scheduler::getInstance().put(t);
    //     } else {
    //         break;
    //     }
    // }
    return 0;
}

void mySemaphore::block(unsigned n) volatile {
    myThread* old = myThread::running;
    old->setBlocked(true);
    old->waitingFor = n;
    blocked.addLast(old);
    myThread::running = Scheduler::getInstance().get();
    myThread::contextSwitch(&old->context, &myThread::running->context);
}