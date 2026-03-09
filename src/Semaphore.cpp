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
    //Riscv::closedSemaphores->addLast(this);

    if (!blocked.getFirst()) return 0;

    while (blocked.getFirst()) {
        blocked.getFirst()->setBlocked(false);
        Scheduler::getInstance().put(blocked.getFirst());
        blocked.removeFirst();
    }

    return 0;
}

int mySemaphore::sem_open(mySemaphore **handle, unsigned init) {
    *handle = new mySemaphore(init);
    return 0;
}

int mySemaphore::sem_wait() {
    if (this->state == CLOSED) {
        return -1;
    }

    if (--value < 0) {
        myThread::running->setBlocked(true); // blokiram trenutnu nit
        blocked.addLast(myThread::running); // vracam je na kraj
        thread_dispatch(); // promena konteksta
    }

    return 0;
}

int mySemaphore::sem_signal() {
    if (this->state == CLOSED) {
        return -1;
    }

    if (value++ <= 0) {
        myThread *deblocked = blocked.getFirst();
        if (!deblocked) return -1;
        deblocked->setBlocked(false);
        Scheduler::getInstance().put(deblocked);
    }

    return 0;
}


// void mySemaphore::wait () {
//     lock();
//     if (--value<0)
//         block();
//     unlock();
// }
//
// void mySemaphore::signal () {
//     lock();
//     if (value++<0)
//         deblock();
//     unlock();
// }

// void signalWait (Semaphore* s, Semaphore* w) {
//     lock();
//     if (s && s->value++<0) s->deblock();
//     if (w && --w->value<0) w->block();
//     unlock();
// }