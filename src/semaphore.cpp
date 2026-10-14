//
// Created by os on 9/2/26.
//
#include "../h/semaphore.hpp"
#include "../lib/hw.h"
#include "../h/list.hpp"
#include "../h/MemoryAllocator.h"
#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"

void Sem::operator delete(void* p) {
    MemoryAllocator::mem_free(p);
}

void *Sem::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}

void Sem::sem_close() {
    TCB* tcb = nullptr;
    while ((tcb = waitingQueue.peekItem()) != nullptr) {
        tcb = waitingQueue.popItem();
        if (tcb) {
            tcb->setRemainingWait(0);
            tcb->setState(TCB::READY);
            Scheduler::put(tcb);
        }
    }
    delete this;
}

Sem *Sem::sem_open(int initValue) {
    Sem* newSem=new Sem(initValue);
    if (!newSem) return nullptr;
    newSem->value=initValue;
    return newSem;
}

void Sem::sem_wait() {
    if (this->value < 1) {
        TCB::runningThread->setRemainingWait(1);
        TCB::runningThread->setState(TCB::BLOCKED);
        this->waitingQueue.addItem(TCB::runningThread);
        TCB::dispatch();
    } else {
        this->value--;
    }
}

void Sem::sem_signal() {
    this->value++;

    TCB* first = waitingQueue.peekItem();
    if (first && first->getRemainingWait() <= this->value) {
        int needed = first->getRemainingWait();
        TCB* tcb = waitingQueue.popItem();
        this->value -= needed;
        tcb->setRemainingWait(0);
        tcb->setState(TCB::READY);
        Scheduler::put(tcb);
    }
}

void Sem::sem_waitN(int n) {
    if (this->value<n){
        TCB::runningThread->setRemainingWait(n);
        TCB::runningThread->setState(TCB::BLOCKED);
        this->waitingQueue.addItem(TCB::runningThread);
        TCB::dispatch();
    }
    else {
        this->value-=n;
    }
}

void Sem::sem_signalN(int n) {
    this->value+=n;
    TCB* first = waitingQueue.peekItem();
    while (first && first->getRemainingWait()<=this->value)
    {
        TCB* tcb=this->waitingQueue.popItem();
        value-=tcb->getRemainingWait();
        tcb->setRemainingWait(0);
        tcb->setState(TCB::READY);
        first=waitingQueue.peekItem();
        Scheduler::put(tcb);
    }
}
