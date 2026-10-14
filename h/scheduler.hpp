//
// Created by os on 25. 8. 2026..
//

#ifndef BASE_PROJECT_SCHEDULER_H
#define BASE_PROJECT_SCHEDULER_H

#include  "list.hpp"

class TCB;

class Scheduler {
    static List readyQueue;

public:
    static void put(TCB* t);
    static TCB* get();
};

#endif //BASE_PROJECT_SCHEDULER_H