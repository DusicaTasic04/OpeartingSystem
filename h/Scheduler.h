//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_SCHEDULER_H
#define PROJECT_BASE_V1_2_SCHEDULER_H

#include "List.h"

class Thread;

class Scheduler {
public:
    static Scheduler& getInstance();

    void put(Thread* thread);
    Thread* get();

private:
    Scheduler();
    List<Thread> readyThreadQueue;
};


#endif //PROJECT_BASE_V1_2_SCHEDULER_H