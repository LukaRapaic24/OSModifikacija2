#include "../h/syscall_cpp.hpp"

Thread::Thread(void (*body)(void *) , void *arg) {
    this->body = body;
    this->arg = arg;
    thread_create(&myHandle,body, arg);
}

Thread::Thread() {
    arg = this;
    body = wrapper;
}

Thread::~Thread() {

}

int Thread::start() {
    return thread_create(&myHandle, this->body, this->arg);
}

void Thread::dispatch() {
    thread_dispatch();
}

void Thread::wrapper(void *arg) {
    Thread *thread = (Thread*)arg;
    thread -> run();
}

int Thread::sleep(time_t t) {
    return -1;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
    return 0;
}

int Semaphore::signal() {
    return sem_signal(myHandle);
    return 0;
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
    return 0;
}

char Console::getc() {
    return ::getc();
    return 0;
}

void Console::putc(char c) {
    ::putc(c);
}