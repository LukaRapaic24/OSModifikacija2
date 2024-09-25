#include "../h/riscv.hpp"

#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/syscall_c.h"
#include "../lib/mem.h"
#include "../h/Semaphore2.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{

    volatile uint64 opcode, arg1, arg2, arg3, ret = 0;
    __asm__ volatile ("mv %0, a0" : "=r" (opcode));
    __asm__ volatile ("mv %0, a1" : "=r" (arg1));
    __asm__ volatile ("mv %0, a2" : "=r" (arg2));
    __asm__ volatile ("mv %0, a3" : "=r" (arg3));

    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch (opcode) {
            case MEM_ALLOC: {
                //ret = (uint64)__mem_alloc((size_t) arg1 * MEM_BLOCK_SIZE);
                ret = (uint64)MemoryAllocator::allocate((size_t)arg1);
                break;
            }
            case MEM_FREE: {
                //ret = (uint64)__mem_free((void*) arg1);
                ret = (uint64)MemoryAllocator::deallocate((void*)arg1);
                break;
            }
            case THREAD_CREATE: {
                TCB* tcb = TCB::createThread((TCB::Body)arg2, (void*)arg3);
                *((TCB**)(arg1)) = tcb;
                ret = 0;
                break;
            }
            case THREAD_EXIT: {
                TCB::exit();
                TCB::dispatch();
                ret = 0;
                break;
            }
            case THREAD_DISPATCH: {
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
            }
            case THREAD_GETID: {
                ret = TCB::getThreadId();
                break;
            }
            case SEM_OPEN: {
                Gate* gate = Gate::openSemaphore((int) arg2);
                *((Gate**)(arg1)) = gate;
                ret = (uint64)gate;
                break;
            }
            case SEM_CLOSE: {
                Gate * gate = (Gate*)arg1;
                ret = Gate::closeSemaphore(gate);
                break;
            }
            case SEM_WAIT: {
                Gate * gate = (Gate*)arg1;
                ret = Gate::waitSemaphore(gate);
                break;
            }
            case SEM_SIGNAL: {
                Gate * gate = (Gate*)arg1;
                ret = Gate::signalSemaphore(gate);
                break;
            }
            case SEM_TRYWAIT: {
                Gate * gate = (Gate*)arg1;
                ret = Gate::trywaitSemaphore(gate);
                break;
            }
            case GETC: {
                ret = __getc();
                break;
            }
            case PUTC: {
                __putc((char)arg1);
                break;
            }
            default: {
                // printString("ERROR: OPCODE does not exist\n");
            }
        }

        __asm__ volatile ( "csrw sscratch, %0" : : "r"( ret ) );

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
        printStringLR("unexpected trap cause\n");
    }
}