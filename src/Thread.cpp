//
// Created by os on 3/7/26.
//

#include "../h/Thread.h"
#include "../h/Riscv.h"

myThread *myThread::main = nullptr;
myThread *myThread::running = nullptr;

myThread *myThread::createThread(Body body, void* arg, uint64* stack) {
    auto *newThread = new myThread(body, arg, stack);
    return newThread;
}

void myThread::yield() {
    dispatch();
}

void myThread::dispatch() {
    myThread *old = running;
    if (!old->isFinished() && !old->isBlocked()) {
        Scheduler::getInstance().put(old);
    }
    running = Scheduler::getInstance().get();

    if (running == main) Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
    else Riscv::mc_sstatus(Riscv::SSTATUS_SPP);

    contextSwitch(&old->context, &running->context);
}

void myThread::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    thread_dispatch();
}

int myThread::exit() {
    // if (running->isFinished())
    //     return -1;

    running->setFinished(true);
    dispatch();
    return 0;
}
