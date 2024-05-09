#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <mutex>

using namespace std;

const int ITERATIONS = 1000000000; // 10^9 разів
int shared_variable;
atomic<int> atomic_shared_variable{0};
atomic<int> counter{0};

void clear_add() {
    for (int i = 0; i < ITERATIONS; ++i) {
        ++shared_variable;
    }
}

void add_with_mutex() {
    static mutex mtx;
    for (int i = 0; i < ITERATIONS; ++i) {
        lock_guard<mutex> lock(mtx);
        ++shared_variable;
    }
}

void add_with_atomic() {
    for (int i = 0; i < ITERATIONS; ++i) {
        ++atomic_shared_variable;
    }
}

void add_synchronously() {
    for (; counter <= ITERATIONS; ++counter) {
        ++atomic_shared_variable;
    }
}

void run_parallel_operations(function<void()> operation, bool flag) {
    auto start_time = chrono::high_resolution_clock::now();

    thread thread1(operation);
    thread thread2(operation);

    thread1.join();
    thread2.join();

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    int value;
    if (flag){
        value = shared_variable;
        shared_variable = 0;
    } else{
        value = atomic_shared_variable;
        atomic_shared_variable = 0;
    }
    cout << "Value: " << value << ", Multiplication took " << duration.count() << " milliseconds." << endl;
}

int main() {
    run_parallel_operations(clear_add, 1);
    run_parallel_operations(add_with_mutex, 1);
    run_parallel_operations(add_with_atomic, 0);
    run_parallel_operations(add_synchronously, 0);

    return 0;
}
