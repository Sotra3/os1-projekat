
//
// Created by os on 25. 8. 2026..
//
#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
List Scheduler::readyQueue;

void Scheduler::put(TCB* t) {
    readyQueue.addItem(t);
}

TCB* Scheduler::get() {
    TCB* t = readyQueue.popItem();
    return t;
}