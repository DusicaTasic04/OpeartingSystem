//
// Created by os on 3/7/26.
//

#include "../h/Thread.h"
#include "../h/Riscv.h"

myThread *myThread::running = nullptr;

myThread *myThread::createThread(Body body, void* arg) {
    myThread *newThread = new myThread(body, arg);
    Scheduler::getInstance().put(newThread);
    return newThread;
}

void myThread::yield() {
    __asm__ volatile ("ecall");
}

void myThread::dispatch() {
    myThread *old = running;
    if (!old->isFinished()) {
        Scheduler::getInstance().put(old);
    }
    running = Scheduler::getInstance().get();

    contextSwitch(&old->context, &running->context);
}

void myThread::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}

void myThread::exit() {
    // TODO: Get this done.
}
