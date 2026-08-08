//
// Created by os on 3/4/26.
//

#include "../h/Thread.h"
#include "../h/workers.hpp"
#include "../h/Semaphore.h"
#include "../h/syscall_c.hpp"
#include "../h/Riscv.h"
#include "../test/printing.hpp"

extern void userMain();

int main() {
    myThread *mainThread, *userMainThread;

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    mainThread = myThread::createThread(nullptr, nullptr, nullptr);
    myThread::main = myThread::running = mainThread;

    thread_create(&userMainThread, reinterpret_cast<void (*)(void *)>(userMain), nullptr);

    while(!userMainThread->isFinished()) {
        thread_dispatch();
    }

    printString("Main finished\n");
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE);
    delete mainThread;
    delete userMainThread;

    Riscv::halt();

    return 0;
}