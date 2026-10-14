//
// Created by os on 25. 8. 2026..
//

#ifndef BASE_PROJECT_TCB_H
#define BASE_PROJECT_TCB_H

#include  "../h/syscall_c.h"
#include  "../h/scheduler.hpp"

class TCB {

public:
    struct Context {
        uint64 sp;
        uint64 ra;
    };
    using Body = void (*)(void*);
    enum State { READY, RUNNING, BLOCKED, FINISHED };

    TCB(Body body, void* arg, char* stackStart)
        : context({stackStart != nullptr ? (uint64)stackStart + DEFAULT_STACK_SIZE : 0,
                   (uint64)&trampoline}),
          stackStart(stackStart),
          state(READY),
          body(body),
          arg(arg)
    {
        if (body != nullptr) {
            Scheduler::put(this);
        }
    }

    ~TCB() { delete []stackStart; };
    // tcb.h, dodaj:
    static void* operator new(size_t size);
    static void operator delete(void* p);
    static void contextSwitch(Context* oldThread, Context* newThread);
    static TCB *createThread(Body body, void* arg, char* stackStart);
    static void threadExit();
    static void dispatch();
    static void trampoline();
    static TCB* runningThread;
    State getState() const { return state; }
    void setState(State s) { state = s; }
    int getRemainingWait() const { return remainingWait; }
    void setRemainingWait(int n) { remainingWait = n; }
private:
    Context context;
    char* stackStart;
    State state;
    Body body;
    void* arg;
    int remainingWait=0;
};

#endif //BASE_PROJECT_TCB_H
