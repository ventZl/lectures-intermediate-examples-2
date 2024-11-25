#include <thread>
#include <semaphore>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <semaphore>

using namespace std::chrono_literals;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(0);
std::vector<int> data;

using vec_iterator = std::vector<int>::iterator;

bool running = true;

int main(int, char **) {
    data.resize(1024);

    while (running) {
        for (auto & d : data) {
            d = distrib(gen);
        }

        /* TODO: Here data is filled with unsorted numbers.
         * We want them to be sorted. While this is done, we would like
         * to do some other useful work (represented by the sleep below)
         *
         * Fix this code to perform sorting in threads so that main thread
         * can do something useful while sorting is being done and as many
         * threads as many CPUs the machine has is used to perform the sorting.
         *
         * Sorting will be performed in parallel in multiple threads to speed
         * up the action. Make sure that if more than CPU count threads were
         * launched, they won't compete with each other and surplus threads
         * will wait for threads already on flight to finish befure starting
         * their workload. This is to mitigate overhead from extensive context
         * switching.
         *
         * Implement the fix in a way the worker thread will be reusable
         * for multiple sort actions. Use std::counting_semaphore.
         */
        std::sort(data.begin(), data.end());

        std::cout << "Sorter numbers: ";
        for (auto d : data) {
            std::cout << d << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
