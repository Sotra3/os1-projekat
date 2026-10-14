//
// Created by os on 25. 8. 2026..
//
#include  "../h/syscall_c.h"

class _thread;
typedef _thread* thread_t;

class _sem;
typedef _sem* sem_t;

void* mem_alloc(uint64 size) {
    size = (size / MEM_BLOCK_SIZE) + (size % MEM_BLOCK_SIZE ? 1 : 0);

    register uint64 a0 asm("a0") = SyscallC::MEMALLOC;
    register uint64 a1 asm("a1") = size;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1) : "memory");

    return (void*) a0;
}

int mem_free(void* ptr) {
    register uint64 a0 asm("a0") = SyscallC::MEMFREE;
    register uint64 a1 asm("a1") = (uint64)ptr;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1) : "memory");

    return (int) a0;
}

int thread_create(thread_t* handle, void (*start_routine)(void*), void* arg) {
    char* stack = (char*)mem_alloc(DEFAULT_STACK_SIZE);

    register uint64 a0 asm("a0") = SyscallC::THREADCREATE;
    register uint64 a1 asm("a1") = (uint64)handle;
    register uint64 a2 asm("a2") = (uint64)start_routine;
    register uint64 a3 asm("a3") = (uint64)arg;
    register uint64 a4 asm("a4") = (uint64)stack;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a3), "r"(a4) : "memory");

    return (int) a0;
}

int thread_exit() {
    register uint64 a0 asm("a0") = SyscallC::THREADEXIT;

    __asm__ volatile ("ecall" : "+r"(a0) : : "memory");

    return (int) a0;
}

void thread_dispatch() {
    register uint64 a0 asm("a0") = SyscallC::DISPATCH;

    __asm__ volatile ("ecall" : "+r"(a0) : : "memory");
}
char getc(){
    uint64 volatile ret = 0;

    __asm__ volatile ("mv a0, %0" : : "r" (SyscallC::GETC));
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %0, a0" : "=r" (ret));

    return (char)ret;
}


void putc(char c){
    __asm__ volatile ("mv a1, %0" : : "r" (c));
    __asm__ volatile ("mv a0, %0" : : "r" (SyscallC::PUTC));
    __asm__ volatile ("ecall");
}

int sem_open(sem_t* handle, unsigned init) {
    register uint64 a0 asm("a0") = SyscallC::SEMOPEN;
    register uint64 a1 asm("a1") = (uint64)handle;
    register uint64 a2 asm("a2") = init;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2) : "memory");

    return (int) a0;
}

int sem_close(sem_t handle) {
    register uint64 a0 asm("a0") = SyscallC::SEMCLOSE;
    register uint64 a1 asm("a1") = (uint64)handle;
    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1) : "memory");
    return (int) a0;
}

int sem_wait(sem_t id) {
    register uint64 a0 asm("a0") = SyscallC::SEMWAIT;
    register uint64 a1 asm("a1") = (uint64)id;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1) : "memory");

    return (int) a0;
}

int sem_signal (sem_t id) {
    register uint64 a0 asm("a0") = SyscallC::SEMSIGNAL;
    register uint64 a1 asm("a1") = (uint64)id;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1) : "memory");

    return (int) a0;
}

int sem_wait_n (sem_t id, unsigned n) {
    register uint64 a0 asm("a0") = SyscallC::SEMWAITN;
    register uint64 a1 asm("a1") = (uint64)id;
    register uint64 a2 asm("a2") = n;
    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2) : "memory");
    return (int) a0;
}

int sem_signal_n (sem_t id, unsigned n) {
    register uint64 a0 asm("a0") = SyscallC::SEMSIGNALN;
    register uint64 a1 asm("a1") = (uint64)id;
    register uint64 a2 asm("a2") = n;

    __asm__ volatile ("ecall" : "+r"(a0) : "r"(a1), "r"(a2) : "memory");

    return (int) a0;
}

int time_sleep (time_t) {
    return 0;
}