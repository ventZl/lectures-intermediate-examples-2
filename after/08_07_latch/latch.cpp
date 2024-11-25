#include <thread>
#include <semaphore>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <semaphore>
#include <latch>

using namespace std::chrono_literals;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(0);
std::binary_semaphore sem_input(1);
std::binary_semaphore sem_output(1);
std::vector<int> data;

using vec_iterator = std::vector<int>::iterator;

bool running = true;

const int partitions = 4;

void thread_worker(std::stop_token token) {
    while (!token.stop_requested()) {
        sem_input.acquire();

        std::latch dwarf_latch(partitions);
        std::vector<std::jthread> dwarfs;
        auto range_begin = data.begin();

        unsigned int boundary = 0;
        auto slice = data.size() / partitions;

        for (int q = 0; q < partitions; ++q) {
            boundary += slice;
            auto range_end = data.begin() + boundary;

            if (q == partitions - 1)
            {
                range_end = data.end();
            }

            dwarfs.emplace_back(std::jthread([] (std::latch & latch, vec_iterator from, vec_iterator to) {
                std::sort(from, to);
                latch.count_down();
            }, std::ref(dwarf_latch), range_begin, range_end));

            range_begin = range_end;
        }

        dwarf_latch.wait();
        std::sort(data.begin(), data.end());
        sem_output.release();
    }

}

int main(int, char **) {
    data.resize(1024);

    std::jthread thread(thread_worker);

    while (running) {
        for (auto & d : data) {
            d = distrib(gen);
        }

        /* Do something interesting here, while waiting for the worker thread */

        std::cout << "Sorter numbers: ";
        for (auto d : data) {
            std::cout << d << ", ";
        }
        std::cout << std::endl;
    }

    thread.request_stop();
    thread.join();
    return 0;
}