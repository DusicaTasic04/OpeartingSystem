//
// Created by os on 3/11/26.
//

#include "../h/workers.hpp"

#include "../lib/hw.h"
#include "../h/Thread.h"
#include "../h/Semaphore.h"
#include "../test/printing.hpp"

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 4 == 0) thread_dispatch();
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void workerBodyA(void*) {
    uint8 i = 0;
    for (; i < 3; i++) {
        printString("A: i=");
        printInt(i);
        printString("\n");
    }

    printString("A: yield\n");
    __asm__ ("li t1, 7");
    thread_dispatch();

    uint64 t1 = 0;
    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));

    printString("A: t1=");
    printInt(t1);
    printString("\n");

    uint64 result = fibonacci(20);
    printString("A: fibonaci=");
    printInt(result);
    printString("\n");

    for (; i < 6; i++) {
        printString("A: i=");
        printInt(i);
        printString("\n");
    }

    myThread::running->setFinished(true);
    thread_dispatch();
}

void workerBodyB(void*) {
    uint8 i = 10;
    for (; i < 13; i++) {
        printString("B: i=");
        printInt(i);
        printString("\n");
    }

    printString("B: yield\n");
    __asm__ ("li t1, 5");
    thread_dispatch();

    uint64 result = fibonacci(23);
    printString("A: fibonaci=");
    printInt(result);
    printString("\n");

    for (; i < 16; i++) {
        printString("B: i=");
        printInt(i);
        printString("\n");
    }

    myThread::running->setFinished(true);
    thread_dispatch();
}

static int a = 0;

void workerBodyC(void*) {
    for (int i = 0; i < 10; i++) {
        a += 1000;
        printInt(a);
        printString("\n");
        thread_dispatch();
    }
    myThread::running->setFinished(true);
    thread_dispatch();
}

void workerBodyD(void*) {
    for (int i = 0; i < 10; i++) {
        a -= 1000;
        printInt(a);
        printString("\n");
        thread_dispatch();
    }
    myThread::running->setFinished(true);
    thread_dispatch();
}
