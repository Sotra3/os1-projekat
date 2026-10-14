//
// Created by os on 9/2/26.
//

#ifndef BASE_PROJECT_SEMAPHORE_HPP
#define BASE_PROJECT_SEMAPHORE_HPP
#include "../lib/hw.h"
#include "../h/list.hpp"

class Sem {
    public:
    static Sem* sem_open(int initValue);
    void sem_close();
    void sem_wait();
    void sem_signal();
    void sem_signalN(int n);
    void sem_waitN(int n);

    void* operator new(size_t size);
    void operator delete(void* p);

private:
    int value;
    List waitingQueue;

    Sem(int initValue) : value(initValue) {}
};

#endif //BASE_PROJECT_SEMAPHORE_HPP
