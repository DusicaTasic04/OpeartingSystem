//
// Created by os on 3/8/26.
//

#include "../h/Riscv.h"
#include "../h/Thread.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/Semaphore.h"
#include "../test/printing.hpp"
#include "../lib/hw.h"

using Body = void (*)(void*);

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::halt() {
    uint32 value = 0x5555;
    auto* address = (uint32*)(0x100000UL);
    *address = value;
}

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();

    if (scause == ECALL_USER || scause == ECALL_SUPER) {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        int returnValue;
        //size_t returnSize;
        mySemaphore* semHandlePtr;
        uint64 code = r_a0();

        switch (code) {
            case MEM_ALLOC:
                size_t size;
                void* ptr;
                __asm__ volatile("mv %0, a1" : "=r" (size));
                ptr = MemoryAllocator::getInstance()->mem_alloc(size * MEM_BLOCK_SIZE);

                __asm__ volatile("mv t0, %0" :: "r" (ptr));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case MEM_FREE:
                void* memptr;
                __asm__ volatile("mv %0, a1" : "=r" (memptr));

                returnValue = MemoryAllocator::getInstance()->mem_free(memptr);

                __asm__ volatile("mv t0, %0" : : "r"(returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case THREAD_CREATE:
                myThread** t;
                Body body;
                void* arg;
                uint64* stack;

                __asm__ volatile ("mv %0, a1" : "=r" (t));
                __asm__ volatile ("mv %0, a2" : "=r" (body));
                __asm__ volatile ("mv %0, a6" : "=r" (stack));
                __asm__ volatile ("mv %0, a7" : "=r" (arg));
                *t = myThread::createThread(body, arg, stack);

                if (*t)
                    __asm__ volatile ("li t0, 0");
                else
                    __asm__ volatile ("li t0, -1");

                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case THREAD_EXIT:
                returnValue = myThread::exit();

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case THREAD_DISPATCH:
                myThread::dispatch();
                break;

            case SEM_OPEN:
                unsigned init;
                mySemaphore** semHandle;

                __asm__ volatile ("mv %0, a2" : "=r" (init));
                __asm__ volatile ("mv %0, a1" : "=r" (semHandle));
                *semHandle = mySemaphore::createSemaphore(init);

                if(*semHandle != nullptr)
                    __asm__ volatile ("li t0, 0");
                else
                    __asm__ volatile ("li t0, -1");;

                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_CLOSE:
                __asm__ volatile ("mv %0, a1" : "=r" (semHandlePtr));
                if(semHandlePtr) {
                    returnValue = semHandlePtr->sem_close();
                }
                else returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_WAIT:
                __asm__ volatile ("mv %0, a1" : "=r" (semHandlePtr));
                if(semHandlePtr) {
                    returnValue = semHandlePtr->sem_wait();
                }
                else returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_SIGNAL:
                __asm__ volatile ("mv %0, a1" : "=r" (semHandlePtr));
                if(semHandlePtr)
                    returnValue = semHandlePtr->sem_signal();
                else
                    returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_WAIT_N:
                unsigned n;
                __asm__ volatile ("mv %0, a6" : "=r" (n));
                __asm__ volatile ("mv %0, a3" : "=r" (semHandlePtr));
                if(semHandlePtr)
                    returnValue = semHandlePtr->sem_wait_n(n);
                else
                    returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_SIGNAL_N:
                __asm__ volatile ("mv %0, a6" : "=r" (n));
                __asm__ volatile ("mv %0, a3" : "=r" (semHandlePtr));
                if(semHandlePtr)
                    returnValue = semHandlePtr->sem_signal_n(n);
                else
                    returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case PUTC:
                char c;
                __asm__ volatile ("mv %0, a1" : "=r" (c));
                __putc(c);
                break;

            case GETC:
                returnValue = __getc();
                __asm__ volatile ("mv t0, %0" : : "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
        return;
    }
    else if (scause == SOFTWARE) {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        return;
    }
    else if (scause == EXTERNAL) {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
        return;
    }
    else {
        // unexpected trap cause
        printString("Error code: ");
        printInt(scause);
        printString(" (BNT = ");
        printInt(scause >> 63);
        printString(")\n");

        printString(" sepc=");
        printInt(Riscv::r_sepc());
        printString(" stval=");
        printInt(Riscv::r_stval());
        printString("\n");
        return;
    }
}