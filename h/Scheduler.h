//
// Created by os on 3/7/26.
//

#ifndef PROJECT_BASE_V1_2_SCHEDULER_H
#define PROJECT_BASE_V1_2_SCHEDULER_H

#include "List.h"

class myThread;

class Scheduler {
public:
    static Scheduler& getInstance();

    void put(myThread* thread);
    myThread* get();

private:
    Scheduler();
    List<myThread> readyThreadQueue;
};


#endif //PROJECT_BASE_V1_2_SCHEDULER_H