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

bool running = true;

int main(int, char **) {
    data.resize(1024);

    while (running) {
        for (auto & d : data) {
            d = distrib(gen);
        }

        std::cout << "Unsorted numbers: ";
        for (auto d : data) {
            std::cout << d << " \t";
        }
        std::cout << std::endl;

        /* TODO: Here data is filled with unsorted numbers.
         * We want them to be sorted. While this is done, we would like
         * to do some other useful work (represented by the sleep below)
         *
         * Fix this code to perform sorting in different thread so that
         * main thread can do something useful while sorting is being done.
         *
         * Implement the fix in a way the worker thread will be reusable
         * for multiple sort actions.
         */

        std::sort(data.begin(), data.end());

        /* Do something interesting here, while waiting for the worker thread */

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "Sorted numbers: ";
        for (auto d : data) {
            std::cout << d << " \t";
        }
        std::cout << std::endl;
    }

    return 0;
}
