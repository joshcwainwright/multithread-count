#ifndef COUNTERTHREAD_H
#define COUNTERTHREAD_H

#include "Thread.h"
#include <mutex>
#include <atomic>
#include <vector>

// thread save
extern std::atomic<int> globalCount;
extern std::mutex printMutex;
extern int g_maxTotal, g_randMin, g_randMax;

// struct to hold thread commit information
struct LogEntry {
    int tid;
    int add;
    int start;
    int end;
};

// buffer to hold commit history index by globalValue
extern std::vector<LogEntry> historyByNew;

class CounterThread final : public Thread {
public:
    explicit CounterThread(int tid);

    // accessors
    int threadId() const { return tid; }
    int contribution() const { return localCount; }

protected:
    // thread work to implement counter
    void run() override;

    // runs when thread work completes
    void done() override;

private:
    // thread id
    int tid;
    // total amount contributed to global count
    int localCount;
};

// helper function to print commit history
void printCommitHistoryInOrder();

#endif
