//
// Created by os on 9/9/26.
//
#include "../h/tcb.hpp"
#include "../h/syscall_cpp.hpp"
#include "../lib/console.h"
#include "../test/printing.hpp"

static Semaphore* start = nullptr;
static Semaphore* finished = nullptr;

void testMain2(void*) {
    int tid=TCB::runningThread->getId();
    printString("Nit ");
    printInt(tid,10,1);
    printString(" ceka pocetak.");
    __putc('\n');
    start->wait();
    printString("Nit ");
    printInt(tid,10,1);
    printString(" pocela.");
    __putc('\n');

    for (int i=0;i<100000;i++){}

    printString("Nit ");
    printInt(tid,10,1);
    printString(" zavrsila.");
    __putc('\n');
    finished->signal();
}


void testMain() {
    start = new Semaphore(0);
    finished = new Semaphore(0);

    Thread* threads[5];

    for (int i=0;i<5;i++) {
        threads[i]=new Thread(testMain2,nullptr);
        threads[i]->start();
    }

    start->signalN(5);

    finished->waitN(5);

    printString("Glavna nit gotova");
}