//
// Created by os on 3/7/26.
//

#include "../h/Thread.h"

//#include "../h/riscv.hpp"

Thread *Thread::running = nullptr;

uint64 Thread::timeSliceCounter = 0;

Thread *Thread::createThread(Body body) {
    return new Thread(body, TIME_SLICE);
}

void Thread::yield() {
    __asm__ volatile ("ecall");
}

void Thread::dispatch()
{
    Thread *old = running;
    if (!old->isFinished()) { Scheduler::getInstance().put(old); }
    running = Scheduler::getInstance().get();

    Thread::contextSwitch(&old->context, &running->context);
}

void Thread::threadWrapper() {
    return;
    //Riscv::popSppSpie();
    running->body();
    running->setFinished(true);
    Thread::yield();
}

void Thread::exit() {

}
