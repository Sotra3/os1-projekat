# OS1 Kernel

A small multithreaded kernel for RISC-V (RV64IMA), built for the
Operating Systems 1 course (School of Electrical Engineering,
University of Belgrade, 2025/26).

## Implemented
- Memory allocator
- Threads (creation, scheduling, context switching)
- Semaphores

## Architecture
Three interface layers:
ABI (`ecall`) → C API (`syscall_c.hpp`) → C++ API (`syscall_cpp.hpp`)

The kernel is statically linked with the user application and runs
on a modified xv6 host inside the QEMU emulator.

## Tech
C++, RISC-V assembly, xv6, QEMU

## Build & Run
(add your build and QEMU launch commands here)
