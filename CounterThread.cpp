#include "CounterThread.h"
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <random>


std::atomic<int> globalCount(0); // to keep track of global value
std::mutex printMutex; // to lock cout
std::vector<LogEntry> historyByNew; // vector to hold commit history

// defaults
int g_maxTotal = 100;
int g_randMin = 1;
int g_randMax = 5;


// constructor with initialization
CounterThread::CounterThread(const int tid) : tid(tid), localCount(0) {
}

void CounterThread::run() {
    thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(g_randMin, g_randMax);

    while (isRunning()) {
        int increment = dist(rng);
        int oldValue = globalCount.load();

        while (isRunning()) {
            // return if reached 100
            if (oldValue >= g_maxTotal) return;
            // check available space until 100
            int room = g_maxTotal - oldValue;
            const int take = (increment <= room) ? increment : room;
            const int end = oldValue + take;

            // attempts to add to global count if the value hasn't changed since last read, otherwise retry
            if (globalCount.compare_exchange_weak(oldValue, oldValue + take)) {
                localCount += take;

                // log history based on end value
                historyByNew[end] = LogEntry{tid, take, oldValue, end};

                break;
            }
        }

        usleep(1000); // prevent single thread from hogging cpu
    }
}

void CounterThread::done() {
}

// guarantees commit history in order
void printCommitHistoryInOrder() {
    const int finalValue = globalCount.load();
    for (int v = 1; v <= finalValue; ++v) {
        const auto &[tid, add, start, end] = historyByNew[v];
        if (add == 0) continue; // skip empty slots
        std::cout << "Thread " << tid
                << " added " << add
                << " to " << start
                << " (global=" << end << ")\n";
    }
}

