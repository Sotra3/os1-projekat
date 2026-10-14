//
// Created by os on 24. 8. 2026..
//
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.h"
#include "../h/semaphore.hpp"

void Riscv::handleSupervisorTrap() {
    uint64 scause=r_scause();

    if (scause==8 || scause==9) {
        volatile uint64 sepc = r_sepc() + 4;
        volatile uint64 sstatus=r_sstatus();
        uint64 ecall_code;
        __asm__ volatile("ld %0, 10*8(fp)" : "=r"(ecall_code));

        switch (ecall_code) {
            case SyscallC::MEMALLOC : {
                uint64 size;
                __asm__ volatile("ld %0, 11*8(fp)" : "=r"(size));

                void* ptr=MemoryAllocator::mem_alloc((size_t)(size*MEM_BLOCK_SIZE));

                __asm__ volatile("sd %0, 10*8(fp)" : : "r"(ptr));
                break;
            }
            case SyscallC::MEMFREE : {
                uint64 ptr;
                __asm__ volatile("ld %0, 11*8(fp)" : "=r"(ptr));

                int ret=MemoryAllocator::mem_free((void*)ptr);

                __asm__ volatile("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::THREADCREATE : {
                uint64 handle,start_routine,arg,stack_space;
                uint64 ret=0;

                __asm__ volatile ("ld %0, 14*8(fp)" : "=r"(stack_space));
                __asm__ volatile ("ld %0, 13*8(fp)" : "=r"(arg));
                __asm__ volatile ("ld %0, 12*8(fp)" : "=r"(start_routine));
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(handle));

                TCB* newTCB=TCB::createThread((TCB::Body)start_routine, (void*)arg, (char*)stack_space);
                *((TCB**)handle) = newTCB;

                __asm__ volatile("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::THREADEXIT : {
                uint64 ret=0;
                TCB::threadExit();
                __asm__ volatile("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::DISPATCH : {
                TCB::dispatch();
                break;
            }
            case SyscallC::SEMOPEN: {
                uint64 handle, initVal;
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(handle));
                __asm__ volatile ("ld %0, 12*8(fp)" : "=r"(initVal));

                Sem* newSem = Sem::sem_open((int)initVal);
                *((Sem**)handle) = newSem;

                uint64 ret = 0;
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::SEMCLOSE : {
                uint64 handle;
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(handle));

                ((Sem*)handle)->sem_close();

                uint64 ret = 0;
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::SEMWAIT : {
                uint64 id;
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(id));
                ((Sem*)id)->sem_wait();
                uint64 ret = 0;
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::SEMSIGNAL : {
                uint64 id;
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(id));
                ((Sem*)id)->sem_signal();
                uint64 ret = 0;
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::SEMWAITN : {
                uint64 id,n;
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(id));
                __asm__ volatile ("ld %0, 12*8(fp)" : "=r"(n));
                ((Sem*)id)->sem_waitN(n);
                uint64 ret = 0;
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::SEMSIGNALN : {
                uint64 id,n;
                __asm__ volatile ("ld %0, 11*8(fp)" : "=r"(id));
                __asm__ volatile ("ld %0, 12*8(fp)" : "=r"(n));
                ((Sem*)id)->sem_signalN(n);
                uint64 ret = 0;
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r"(ret));
                break;
            }
            case SyscallC::GETC : {
                char c = __getc();
                __asm__ volatile ("sd %0, 10*8(fp)" : : "r" (c));
                break;
            }
            case SyscallC::PUTC : {
                char c;
                __asm__ volatile ("ld %0, 11 * 8(fp)" : "=r" (c));
                __putc(c);
                break;
            }
            default:
                break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001ULL)
    {
        mc_SIP(SIP_SSIP);
    }
    else if (scause == 0x8000000000000009ULL)
    {
        console_handler();
    }
    else
    {
        char chScause = scause + '0';
        __putc('E');__putc('R');__putc('R');__putc('O');__putc('R');__putc('\n');
        __putc('S');__putc('C');__putc('A');__putc('U');__putc('S');__putc('E');__putc(' ');__putc(chScause);__putc('\n');
        __putc('\n');
        uint64 sepc_val;
        __asm__ volatile("csrr %0, sepc" : "=r"(sepc_val));
        // odštampaj sepc_val u hex formatu
        __putc('0'); __putc('x');
        for (int i = 60; i >= 0; i -= 4) {
            char c = (sepc_val >> i) & 0xF;
            __putc(c < 10 ? '0' + c : 'A' + c - 10);
        }
        __putc('\n');
        *(int*)0x100000 = 0x5555;
    }
}

uint64 Riscv::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

void Riscv::w_stvec(uint64 addr) {
    __asm__ volatile ("csrw stvec, %0" : : "r"(addr));
}

uint64 Riscv::r_sepc() {
    uint64 sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

void Riscv::w_sepc(uint64 addr) {
    __asm__ volatile ("csrw sepc, %0" : : "r"(addr));
}

uint64 Riscv::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

void Riscv::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %0" : : "r"(sstatus));
}

void Riscv::popSPPSPIE() {
    __asm__ volatile (
        "csrc sstatus, %0\n"
        "csrw sepc, ra\n"
        "sret\n"
        :
        : "r"(SSTATUS_SPP)
        : "memory"
    );
}

void Riscv::mc_SSTATUS(uint64 mask) {
    __asm__ volatile("csrc sstatus, %0" : : "r"(mask));
}

void Riscv::mc_SIP(uint64 mask) {
    __asm__ volatile("csrc sip, %0" : : "r"(mask));
}