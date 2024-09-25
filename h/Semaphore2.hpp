//
// Created by Luka on 19-Sep-2024.
//

#ifndef LUKA_RAPAIC_OS_PROJEKAT_SEMAPHORE2_HPP
#define LUKA_RAPAIC_OS_PROJEKAT_SEMAPHORE2_HPP

#include "../lib/hw.h"
#include "tcb.hpp"

class Gate {
public:

    static Gate* openSemaphore(uint64 init);
    static int closeSemaphore(Gate* gate);
    static int waitSemaphore(Gate* gate);
    static int trywaitSemaphore(Gate* gate);
    static int signalSemaphore(Gate* gate);

private:
    Gate(uint64 init);

    List <TCB> waitingList;
    int balance;
};

#endif //LUKA_RAPAIC_OS_PROJEKAT_SEMAPHORE2_HPP
