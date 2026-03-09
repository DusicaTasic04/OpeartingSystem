//
// Created by os on 3/7/26.
//

#include "../h/Scheduler.h"

Scheduler& Scheduler::getInstance() {
    static Scheduler sched;
    return sched;
}

void Scheduler::put(myThread *thread) {
    readyThreadQueue.addLast(thread);
}

myThread* Scheduler::get() {
    return readyThreadQueue.removeFirst();
}
