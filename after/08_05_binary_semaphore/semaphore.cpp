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

bool running = true;

void thread_worker(std::stop_token token) {
    while (!token.stop_requested()) {
        sem_input.acquire();
        std::sort(data.begin(), data.end());
        sem_output.release();
    }

}

int main(int, char **) {
    data.resize(1024);

    sem_input.acquire();
    sem_output.acquire();

    std::jthread thread(thread_worker);

    while (running) {
        for (auto & d : data) {
            d = distrib(gen);
        }

        std::cout << "Unsorted numbers: ";
        for (auto d : data) {
            std::cout << d << " \t";
        }
        std::cout << std::endl;

        /* Release the input semaphore, input data has been filled in */
        sem_input.release();

        /* Do something interesting here, while waiting for the worker thread */

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        /* Fed up waiting. Let's grab the output data! */
        sem_output.acquire();

        std::cout << "Sorted numbers: ";
        for (auto d : data) {
            std::cout << d << " \t";
        }
        std::cout << std::endl;
    }

    thread.request_stop();
    thread.join();
    return 0;
}
