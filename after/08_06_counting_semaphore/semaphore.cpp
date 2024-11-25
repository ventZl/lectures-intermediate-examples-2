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
std::binary_semaphore sem_input(1);
std::binary_semaphore sem_output(1);
std::vector<int> data;

using vec_iterator = std::vector<int>::iterator;

bool running = true;

const int blocksize = 128;

std::counting_semaphore<32> core_allocator(4);

void thread_worker(std::stop_token token) {
    while (!token.stop_requested()) {
        sem_input.acquire();
        std::vector<std::thread> dwarfs;
        auto range_begin = data.begin();
        unsigned int q = 0;
        do {
            q += blocksize;
            auto range_end = data.begin() + q;
            if (q >= data.size())
            {
                range_end = data.end();
            }
            dwarfs.emplace_back(std::thread([] (vec_iterator from, vec_iterator to) {
                core_allocator.acquire();
                std::sort(from, to);
                core_allocator.release();
            }, range_begin, range_end));

            range_begin = range_end;
        } while (q < data.size());

        for (auto & dwarf : dwarfs) {
            dwarf.join();
        }

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
