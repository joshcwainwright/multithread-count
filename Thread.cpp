#include "Thread.h"
#include <iostream>

// default constructor
Thread::Thread() : running(false) {
};

// default destructor
Thread::~Thread() = default;


void Thread::start() {
    // start new thread
    running = true;
    pthread_create(&thread, nullptr, &Thread::threadEntry, this);
}

void Thread::join() const {
    // waits for thread to finish execution
    pthread_join(thread, nullptr);
}

// stopped flag must be checked in
void Thread::stop() {
    running = false;
}

// must be checked in run for stop to work
bool Thread::isRunning() const {
    return running.load();
}

// returns thread id
pthread_t Thread::id() const {
    // return thread handle
    return thread;
}

void *Thread::threadEntry(void *arg) {
    // casts arg to Thread*
    auto *self = static_cast<Thread *>(arg);
    // runs thread work
    self->run();
    // indicates thread work is concluded
    self->done();
    return nullptr;
}

// default done method
void Thread::done() {
    std::cout << "Thread " << id() << " done.\n";
    running = false;
}
