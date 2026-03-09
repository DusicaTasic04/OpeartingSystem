//
// Created by os on 3/7/26.
//

#include "../h/Thread.h"

#include "../h/Riscv.h"

myThread *myThread::running = nullptr;

uint64 myThread::timeSliceCounter = 0;

myThread *myThread::createThread(Body body) {
    return new myThread(body, TIME_SLICE);
}

void myThread::yield() {
    __asm__ volatile ("ecall");
}

void myThread::dispatch()
{
    myThread *old = running;
    if (!old->isFinished()) {
        Scheduler::getInstance().put(old);
    }
    running = Scheduler::getInstance().get();

    contextSwitch(&old->context, &running->context);
}

void myThread::threadWrapper() {
    Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    myThread::yield();
}

void myThread::exit() {
    // TODO: Get this done.
}
