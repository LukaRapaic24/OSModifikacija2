#include "../h/tcb.hpp"
//#include "../h/print.hpp"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"

extern void userMain();

void userMainWr(void* arg) {
    userMain();
}

int main()
{
    MemoryAllocator::init_memory();

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    TCB *threads[2];
    threads[0] = TCB::createThread(nullptr, nullptr);
    TCB::running = threads[0];
    threads[1] = TCB::createThread(&userMainWr, nullptr);

    /*
    threads[1] = TCB::createThread(workerBodyA, nullptr);
    printString("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB, nullptr);
    printString("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC, nullptr);
    printString("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD, nullptr);
    printString("ThreadD created\n");
*/
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished()))
    {
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    // printString("Finished\n");

    return 0;
}
