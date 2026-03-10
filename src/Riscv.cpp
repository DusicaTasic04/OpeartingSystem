//
// Created by os on 3/8/26.
//

#include "../h/Riscv.h"
#include "../h/Thread.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/Semaphore.h"
#include "../h/Print.h"

using Body = void (*)(void*);

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();

    if (scause == ECALL_USER || scause == ECALL_SUPER) {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 returnValue;
        mySemaphore* semHandlePtr;
        uint64 code = r_a0();
        switch (code) {
            case MEM_ALLOC:
                size_t size;
                void* ptr;
                __asm__ volatile("mv %0, a1" : "=r" (size));
                ptr = MemoryAllocator::getInstance().mem_alloc(size);

                __asm__ volatile("mv t0, %0" : : "r" (ptr));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case MEM_FREE:
                void* memptr;
                __asm__ volatile("mv %0, a1" : "=r" (memptr));

                returnValue = MemoryAllocator::getInstance().mem_free(memptr);

                __asm__ volatile("mv t0, %0" : : "r"(returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case MEM_GET_FREE_SPACE:
                returnValue = MemoryAllocator::getInstance().mem_get_free_space();

                __asm__ volatile ("mv t0, %0" : : "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");

                break;
            case MEM_GET_LARGEST_FREE_BLOCK:
                returnValue = MemoryAllocator::getInstance().mem_get_largest_free_block();

                __asm__ volatile ("mv t0, %0" : : "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");

                break;
            case THREAD_CREATE:
                myThread** t;
                Body body;
                void* arg;

                __asm__ volatile ("mv %0, a1" : "=r" (t));
                __asm__ volatile ("mv %0, a2" : "=r" (body));
                __asm__ volatile ("mv %0, a7" : "=r" (arg));
                *t = myThread::createThread(body, arg);

                if (*t)
                    __asm__ volatile ("li t0, 0");
                else
                    __asm__ volatile ("li t0, -1");

                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case THREAD_EXIT:
                myThread::running->setFinished(true);
                myThread::dispatch();

                __asm__ volatile ("li t0, 0");
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
                *semHandle = new mySemaphore(init);

                if(*semHandle != nullptr)
                    __asm__ volatile ("li t0, 0");
                else
                    __asm__ volatile ("sw t0, 80(x8)");

                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_CLOSE:
                __asm__ volatile ("mv %0, a1" : "=r" (semHandlePtr));
                if(semHandlePtr!= nullptr) {
                    returnValue = semHandlePtr->sem_close();
                }
                else returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_WAIT:
                __asm__ volatile ("mv %0, a1" : "=r" (semHandlePtr));
                if(semHandlePtr!= nullptr) {
                    returnValue = semHandlePtr->sem_wait();
                }
                else returnValue = -2;

                __asm__ volatile ("mv t0, %0" : : "r" (returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case SEM_SIGNAL:
                __asm__ volatile ("mv %0, a1" : "=r" (semHandlePtr));
                if(semHandlePtr!= nullptr)
                    returnValue = semHandlePtr->sem_signal();
                else
                    returnValue = -2;

                __asm__ volatile ("mv t0, %0" :: "r"(returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            case PUTC:
                char c;
                __asm__ volatile ("mv %0, a1" : "=r" (c));
                __putc(c);
                break;

            case GETC:
                returnValue = __getc();
                __asm__ volatile ("mv t0, %0" : : "r"(returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
                break;

            default: // ovo ne bi smelo da se dogodi
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == SOFTWARE) {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
    }
    else if (scause == EXTERNAL) {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else {
        // unexpected trap cause
        printString("ERROR! SCAUSE:");
        printInteger(scause);
        printString("\n");
    }
}