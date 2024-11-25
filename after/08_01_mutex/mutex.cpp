#include <thread>
#include <mutex>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>

using namespace std::chrono_literals;
long counter = 0;
std::mutex mut;

void thread_incrementor(int iterations) {
    while (iterations-- > 0) {
        mut.lock();
        counter = counter + 1;
        mut.unlock();
    }
}

int main(int, char **) {
    std::thread threads[4];
    for (int q = 0; q < 4; ++q) {
        threads[q] = std::thread(thread_incrementor, 100000);
    }

    for (auto & worker : threads) { worker.join(); }
    std::cout << "Counter value is: " << counter << std::endl;
    return 0;
}
