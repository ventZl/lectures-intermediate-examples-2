#include <thread>
#include <mutex>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(1, 10);

using namespace std::chrono_literals;
long counter = 0;
std::mutex mut;

void increment_counter(long & counter) {
    counter = counter + 1;
    if (distrib(gen) == 6) {
        throw std::runtime_error("Error happened");
    }
}

void thread_incrementor(int iterations) {
    try {
        while (iterations-- > 0) {
            std::lock_guard lk(mut);
            increment_counter(counter);
        }
    } catch (...) {
        std::cout << "Error happened! Exiting!" << std::endl;
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
