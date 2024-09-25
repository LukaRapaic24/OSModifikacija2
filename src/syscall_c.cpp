#include "../h/syscall_c.h"

void syscall(uint64 opCode=0, uint64 arg0 = 0, uint64 arg1 = 0, uint64 arg2 = 0, uint64 arg3 = 0, uint64 arg4 = 0, uint64 arg5 = 0, uint64 arg6 = 0) {
    __asm__ volatile ("ecall");
};

void test() {
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");
    return;
}

void* mem_alloc(size_t size) {
    size_t blockCnt = size % MEM_BLOCK_SIZE ? size / MEM_BLOCK_SIZE + 1 : size / MEM_BLOCK_SIZE;
    syscall(MEM_ALLOC, blockCnt);

    void* returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int mem_free(void* address) {
    syscall(MEM_FREE, (uint64) address);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {

    syscall(THREAD_CREATE, (uint64) handle, (uint64) start_routine, (uint64) arg);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int time_sleep(time_t time) {
    syscall(TIME_SLEEP, (uint64) time);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int thread_exit() {
    syscall(THREAD_EXIT);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

void thread_dispatch() {
    syscall(THREAD_DISPATCH);
}

int thread_getId() {
    syscall(THREAD_GETID);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int sem_open (sem_t* handle, unsigned init) {
    syscall(SEM_OPEN, (uint64) handle, init);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int sem_close(sem_t id) {
    syscall(SEM_CLOSE, (uint64) id);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int sem_wait(sem_t id) {
    syscall(SEM_WAIT, (uint64) id);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int sem_signal(sem_t id) {
    syscall(SEM_SIGNAL, (uint64) id);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

int sem_trywait(sem_t id) {
    syscall(SEM_TRYWAIT, (uint64) id);

    int returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

char getc() {
    syscall(GETC);

    char returnValue;
    __asm__ volatile ("mv a0, %0" : "=r" (returnValue));
    return returnValue;
}

void putc(char c) {
    syscall(PUTC, c);
}

