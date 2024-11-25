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
        std::cout << std::endl << std::endl;
    }
};

std::queue<std::shared_ptr<Job>> job_queue;

std::mutex mut;
std::condition_variable cv;

bool ready = false;
bool running = true;

void thread_worker(std::stop_token token) {
    while (true) {
        std::unique_lock lock(mut);
        cv.wait(lock, [&, token] { return ready || token.stop_requested(); });
        if (token.stop_requested())
        {
            break;
        }
        ready = false;
        std::shared_ptr<Job> job = job_queue.front();
        job_queue.pop();
        lock.unlock();

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

        {
            std::lock_guard guard(mut);
            job_queue.push(job);
            ready = true;
            cv.notify_one();
        }
    }

    for (auto & thread : workers) {
        thread.request_stop();
    }

    {
        std::lock_guard guard(mut);
        cv.notify_all();
    }

    return 0;
}
