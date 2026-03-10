//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_THREAD_H
#define PROJECT_BASE_V1_2_THREAD_H


#include "../lib/hw.h"
#include "Scheduler.h"

class myThread {
public:
    ~myThread() { delete[] stack; }

    bool isFinished() const { return finished; }
    void setFinished(const bool value) { finished = value; }

    bool isBlocked() const { return blocked; }
    void setBlocked(const bool value) { blocked = value;}

    using Body = void (*)(void*);

    static myThread *createThread(Body body, void* arg);

    static void yield();

    static myThread *running;

private:
    myThread(Body body, void* arg) :
        body(body),
        stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
        context({(uint64) &threadWrapper,stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0 }),
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