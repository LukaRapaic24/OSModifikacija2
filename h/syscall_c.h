#ifndef LUKA_RAPAIC_OS_PROJEKAT_SYSCALL_C_HPP
#define LUKA_RAPAIC_OS_PROJEKAT_SYSCALL_C_HPP

#include "../lib/hw.h"

constexpr uint64 MEM_ALLOC = 0x01;
constexpr uint64 MEM_FREE = 0x02;
constexpr uint64 THREAD_CREATE = 0x11;
constexpr uint64 THREAD_EXIT = 0x12;
constexpr uint64 THREAD_DISPATCH = 0x13;
constexpr uint64 THREAD_GETID = 0x14;
constexpr uint64 SEM_OPEN = 0x21;
constexpr uint64 SEM_CLOSE = 0x22;
constexpr uint64 SEM_WAIT = 0x23;
constexpr uint64 SEM_SIGNAL = 0x24;
constexpr uint64 SEM_TIMEDWAIT = 0x25;
constexpr uint64 SEM_TRYWAIT = 0x26;
constexpr uint64 TIME_SLEEP = 0x31;
constexpr uint64 GETC = 0x41;
constexpr uint64 PUTC = 0x42;

class TCB;
typedef TCB* thread_t;

class Gate;
typedef Gate* sem_t;

const int EOF = -1;

void syscall(uint64 opCode, uint64 arg1, uint64 arg2, uint64 arg3, uint64 arg4, uint64 arg5, uint64 arg6, uint64 arg7);

void test();

void* mem_alloc(size_t size);

int mem_free(void* address);

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit();
void thread_dispatch();
int time_sleep(time_t);

int sem_open(sem_t* handle, unsigned init);
int sem_close(sem_t id);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int sem_trywait(sem_t id);

char getc();
void putc(char c);



#endif //LUKA_RAPAIC_OS_PROJEKAT_SYSCALL_C_HPP
