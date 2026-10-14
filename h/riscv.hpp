//
// Created by os on 24. 8. 2026..
//

#ifndef BASE_PROJECT_RISCV_H
#define BASE_PROJECT_RISCV_H

#include "../lib/hw.h"

class Riscv {
public:
    static constexpr uint64 SIP_SSIP = (1 << 1); // Supervisor Software Interrupt Pending
    static constexpr uint64 SIP_STIP = (1 << 5);
    static constexpr uint64 SIP_SEIP = (1 << 9);
    static constexpr uint64 SSTATUS_SIE  = (1 << 1); // Supervisor Interrupt Enable
    static constexpr uint64 SSTATUS_SPIE = (1 << 5); // Supervisor Previous Interrupt Enable
    static constexpr uint64 SSTATUS_SPP  = (1 << 8); // Supervisor Previous Privileg

    static void mc_SIP(uint64 mask);
    static void mc_SSTATUS(uint64 mask);
    static void popSPPSPIE();

    static void supervisorTrap();              // implementirano u .S, samo deklaracija ovde
    static void handleSupervisorTrap();          // implementirano u .cpp
    static void w_stvec(uint64 addr);
    static uint64 r_scause();
    static uint64 r_sepc();
    static uint64 r_sstatus();
    static void w_sstatus(uint64 sstatus);
    static void w_sepc(uint64 addr);
};

#endif //BASE_PROJECT_RISCV_H
