#ifndef LUKA_RAPAIC_OS_PROJEKAT_SYSCALL_CPP_HPP
#define LUKA_RAPAIC_OS_PROJEKAT_SYSCALL_CPP_HPP

#include "syscall_c.h"
//#include "../src/_new.cpp"

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*);
    void* arg;
    static void wrapper(void* arg);
};


class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
private:
    sem_t myHandle;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};


#endif //LUKA_RAPAIC_OS_PROJEKAT_SYSCALL_CPP_HPP
