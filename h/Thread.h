//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_THREAD_H
#define PROJECT_BASE_V1_2_THREAD_H


#include "../lib/hw.h"
#include "Scheduler.h"
#include "../h/MemoryAllocator.h"

class myThread {
public:
    ~myThread() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(const bool value) { finished = value; }

    bool isBlocked() const { return blocked; }
    void setBlocked(const bool value) { blocked = value;}

    using Body = void (*)(void*);

    static myThread *createThread(Body body, void* arg, uint64* stack);

    static void yield();

    static myThread *running;
    static myThread *main;

    void* operator new(size_t size) {
        return MemoryAllocator::getInstance().mem_alloc(size);
    }
    void* operator new[](size_t size) {
        return MemoryAllocator::getInstance().mem_alloc(size);
    }

    void operator delete(void *ptr) {
        MemoryAllocator::getInstance().mem_free(ptr);
    }
    void operator delete[](void *ptr) {
        MemoryAllocator::getInstance().mem_free(ptr);
    }

private:
    myThread(Body body, void* arg, uint64 *stack) :
        body(body),
        stack(stack == nullptr ? new uint64[STACK_SIZE] : stack),
        context({(uint64)&threadWrapper, stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0 }),
        arg(arg),
        finished(false),
        blocked(false) {
        if (body != nullptr) { Scheduler::getInstance().put(this); }
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    void* arg;
    bool finished;
    bool blocked;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    int exit();

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
};

#endif //PROJECT_BASE_V1_2_THREAD_H