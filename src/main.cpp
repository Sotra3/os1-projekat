#include "../lib/hw.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"


extern void userMain();

volatile bool userMainFinished = false;

void userMainThread(void*) {
    userMain();
    userMainFinished = true;
}

int main() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB::runningThread = new TCB(0, 0, 0);
    TCB::runningThread->setState(TCB::RUNNING);

    thread_t userMainThreadHandle;
    thread_create(&userMainThreadHandle,userMainThread, nullptr);

    while (!userMainFinished) {
        TCB::dispatch();
    }

    *(int*)0x100000 = 0x5555;
    return 0;
}