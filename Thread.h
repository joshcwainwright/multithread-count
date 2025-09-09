#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <atomic>

class Thread {
public:
    // default constructor
    Thread();

    // virtual destructor
    virtual ~Thread();

    // begins the work associated w the thread
    void start();

    // calls pthread_join on stored thread
    void join() const;

    // stops word associated with thread
    void stop();

    // returns if thread is running
    bool isRunning() const;

    // returns the unique id of current thead once the class is built
    pthread_t id() const;

protected:
    // runs the work associated with the thread. must be implemented
    virtual void run() = 0;

    // called when the work associated thread is complete
    virtual void done();

private:
    // entry point to pthread_create
    static void *threadEntry(void *arg);

    // stores the id of created thread
    pthread_t thread{};

    // stores whether the thread is running
    std::atomic<bool> running{};
};


#endif //THREAD_H
