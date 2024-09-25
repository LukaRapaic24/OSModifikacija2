//
// Created by Luka on 19-Sep-2024.
//

#include "../h/Semaphore2.hpp"

Gate::Gate(uint64 init): balance(init) {}

Gate* Gate::openSemaphore(uint64 init) {
    return new Gate(init);
}
int Gate::waitSemaphore(Gate* gate) {
    gate->balance--;
    if(gate->balance < 0) {
        TCB::running->setBlocked(true);
        gate->waitingList.addLast(TCB::running);
        TCB::yield();
        return -1;
    }
    return 0;
}

int Gate::trywaitSemaphore(Gate* gate) {
    if (gate->balance) {
        gate->balance--;
        return 1;
    } else return 0;
}

int Gate::signalSemaphore(Gate* gate) {
    gate->balance++;
    if(gate->balance <= 0) {
        TCB* trd = gate->waitingList.removeFirst();
        //if(trd != nullptr) {
            trd->setBlocked(false);
            Scheduler::put(trd);
        //}
        return -1;
    }
    return 0;
}

int Gate::closeSemaphore(Gate* gate) {
    while (gate->waitingList.peekFirst()) {
        TCB *trd = gate->waitingList.removeFirst();
        trd->setBlocked(false);
        Scheduler::put(trd);
    }
    return 0;
}