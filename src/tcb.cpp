//
// Created by os on 25. 8. 2026..
//
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/scheduler.hpp"

TCB *TCB::runningThread=nullptr;

TCB *TCB::createThread(Body body, void* arg, char* stackStart) {
    return new TCB(body, arg, stackStart);
}

void TCB::threadExit() {
    runningThread->setState(FINISHED);
    dispatch();
}

void TCB::trampoline() {
    Riscv::popSPPSPIE();
    runningThread->body(runningThread->arg);
    thread_exit();
}

void TCB::dispatch() {
    TCB* old = runningThread;
    if (old == nullptr) return;
    TCB* next = Scheduler::get();

    if (next == nullptr) {
        if (old->getState() == TCB::RUNNING) {
            return;
        }
        return;
    }
    if (old == next) {
        return;
    }
    if (old->getState() == TCB::RUNNING) {
        old->setState(TCB::READY);
        Scheduler::put(old);
    }

    runningThread = next;
    runningThread->setState(TCB::RUNNING);

    contextSwitch(&old->context, &runningThread->context);
}

void* TCB::operator new(size_t size) {
    return MemoryAllocator::mem_alloc(size);
}
void TCB::operator delete(void* p) {
    MemoryAllocator::mem_free(p);
}