#include <thread>
#include <semaphore>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <semaphore>
#include <condition_variable>
#include <memory>
#include <queue>

using namespace std::chrono_literals;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(0);

struct Job {
    std::vector<int> data;

    void callback() {
        std::cout << "Sorted numbers: ";
        for (auto d : data) {
            std::cout << d << " \t";
        }
        std::cout << std::endl;
    }
};

std::queue<std::shared_ptr<Job>> job_queue;

bool running = true;

void thread_worker(std::stop_token token) {
    while (!token.stop_requested()) {
        std::shared_ptr<Job> job = job_queue.front();
        job_queue.pop();

        std::sort(job->data.begin(), job->data.end());
        job->callback();
    }

}

int main(int, char **) {

    std::vector<std::jthread> workers;
    for (unsigned q = 0; q < 4; ++q) {
        workers.emplace_back(std::jthread(thread_worker));
    }

    while (running) {
        auto job = std::make_shared<Job>();
        job->data.resize(1024);

        for (auto & d : job->data) {
            d = distrib(gen);
        }

        job_queue.push(job);
        /* TODO: Add notification mechanism that picks one of the worker threads to
         * perform sorting.
         */
    }

    for (auto & thread : workers) {
        thread.request_stop();
    }

    return 0;
}
