#include "CounterThread.h"
#include <vector>
#include <iostream>
#include <chrono>

int main(int argc, char *argv[]) {
    // defaults
    g_maxTotal = 100;
    g_randMin = 1;
    g_randMax = 5;

    int g_numThreads = 5;

    // parse cli arguments
    try {
        if (argc >= 2) g_maxTotal = std::stoi(argv[1]);
        if (argc >= 3) g_randMin = std::stoi(argv[2]);
        if (argc >= 4) g_randMax = std::stoi(argv[3]);
        if (argc >= 5) g_numThreads = std::stoi(argv[4]);
    } catch (const std::exception &e) {
        std::cerr << "Invalid arguments. Usage: ./app TOTAL [MIN [MAX] [NUM_THREADS]]\n";
        return 1;
    }


    // print start phrase
    if (g_maxTotal < 1 || g_randMin < 1 || g_randMax < g_randMin || g_numThreads < 1) {
        std::cerr << "Usage: ./app [TOTAL>=1] [MIN>=1] [MAX>=MIN] [NUM_THREADS>=1]\n";
        return 1;
    }

    std::cout << "\n" << "Counting to " << g_maxTotal
            << " with " << g_numThreads
            << " threads using random increments between "
            << g_randMin << " and " << g_randMax << ":\n\n";


    // allocate history buffer
    historyByNew.assign(g_maxTotal + 1, LogEntry{});

    // stores pointer to each thread
    std::vector<CounterThread *> threads;

    // record start time
    const auto start = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < g_numThreads; ++i) {
        // creates new thread with unique id
        threads.push_back(new CounterThread(i + 1));
        // starts new thread
        threads.back()->start();
    }

    for (const auto &t: threads) {
        // waits for threads to finish execution
        t->join();
    }

    std::cout << "\n======= Final per-thread contribution =======\n";
    std::ranges::sort(threads,
                      [](auto *a, auto *b) { return a->threadId() < b->threadId(); });
    for (const auto *t: threads) {
        std::cout << "Thread " << t->threadId()
                << " final contribution = " << t->contribution() << "\n";
    }
    std::cout << "=============================================\n\n";

    std::cout << "\n=== Commit history in strict global order ===\n";
    printCommitHistoryInOrder();
    std::cout << "=============================================\n\n";

    // Record end time of all threads
    const auto end = std::chrono::high_resolution_clock::now();

    // prints final value of global value
    std::cout << "Global count reached " << globalCount.load() << "\n";

    // frees thread memory
    for (const auto &t: threads) {
        delete t;
    }

    // find duration in milliseconds
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Total time: " << duration << " ms\n";

    return 0;
}
