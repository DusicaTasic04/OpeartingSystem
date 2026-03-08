//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_THREAD_H
#define PROJECT_BASE_V1_2_THREAD_H


#include "../lib/hw.h"
#include "Scheduler.h"

class Thread {
public:
    ~Thread() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)();

    static Thread *createThread(Body body);

    static void yield();

    static Thread *running;

private:
    Thread(Body body, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false)
    {
        if (body != nullptr) { Scheduler::getInstance().put(this); }
    }

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    friend class Riscv;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    void exit();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;
};

#endif //PROJECT_BASE_V1_2_THREAD_H