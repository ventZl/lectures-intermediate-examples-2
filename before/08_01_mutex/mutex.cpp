#include <thread>
#include <mutex>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>

using namespace std::chrono_literals;
long counter = 0;

void thread_incrementor(int iterations) {
    while (iterations-- > 0) {
        counter = counter + 1;
    }
}

int main(int, char **) {
    std::thread threads[4];
    /* TODO: Fix data races in this code that executes in parallel.
     * Make sure that the counter will contain correct value once
     * threads finish their work.
     */
    for (int q = 0; q < 4; ++q) {
        threads[q] = std::thread(thread_incrementor, 100000);
    }

    for (auto & worker : threads) { worker.join(); }
    std::cout << "Counter value is: " << counter << std::endl;
    return 0;
}
