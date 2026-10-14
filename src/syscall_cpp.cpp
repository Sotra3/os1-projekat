//
// Created by os on 27. 8. 2026..
//
#include "../h/syscall_cpp.hpp"
#include "../lib/console.h"


Semaphore::Semaphore(unsigned int init) {
    sem_open(&this->myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(this->myHandle);
}

int Semaphore::wait() {
    return sem_wait(this->myHandle);
}

int Semaphore::signal() {
    return sem_signal(this->myHandle);
}

int Semaphore::signalN(int n) {
    return sem_signal_n(this->myHandle, n);
}

int Semaphore::waitN(int n) {
    return sem_wait_n(this->myHandle, n);
}


void *operator new(size_t n)
{
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    mem_free(p);
}

void operator delete[](void *p) noexcept
{
    mem_free(p);
}

Thread::Thread(void (*body)(void *), void *arg) : body(body), arg(arg) {
}


Thread::~Thread(){
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() : body(&runWrapper), arg(this){

}

int Thread::sleep(time_t) {
    return 0;
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    ::putc(c);
}