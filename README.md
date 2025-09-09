# Threaded Counter App

This project implements a small C++ threading library that abstracts `pthread` in a Java-like way, plus a demo application that uses it to count to a target value using multiple threads.

## Features

- **Thread abstraction (`Thread` class)** with:
  - `start()` – begin thread execution
  - `stop()` – request thread to stop
  - `done()` – called when thread completes
  - `id()` – return unique thread identifier
- **Counter demo (`CounterThread`)**:
  - Multiple threads compete to increment a shared global counter.
  - Each increment is chosen randomly within a range.
  - Commit history is recorded in strict order.
  - After all threads complete, each thread’s total contribution and the ordered commit history are printed.

## Build Instructions

You need a C++20-capable compiler (e.g. `g++` or `clang++`) and pthread support.

```bash
g++ -std=c++20 -pthread main.cpp Thread.cpp CounterThread.cpp -o app
```

This will produce an executable called `app`.

## Usage

```bash
./app [TOTAL] [MIN] [MAX] [NUM_THREADS]
```

### Arguments

- `TOTAL` – target count value (default: `100`)
- `MIN` – minimum random increment per step (default: `1`)
- `MAX` – maximum random increment per step (default: `5`)
- `NUM_THREADS` – number of worker threads (default: `5`)

### Examples

Run with defaults (count to 100 using 5 threads, increments between 1 and 5):

```bash
./app
```

Count to 200 with increments between 1 and 5:

```bash
./app 200
```

Count to 200 with increments between 2 and 7:

```bash
./app 200 2 7
```

Count to 500 with increments between 1 and 10 using 8 threads:

```bash
./app 500 1 10 8
```
