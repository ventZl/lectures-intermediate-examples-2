#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

class ThreadWorker {
public:
    void run(std::stop_token token, int interval, int from, int to) {
        while (from < to && !token.stop_requested()) {
            std::cout << from++ << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }
};

int main(int, char **) {
    ThreadWorker worker;
    std::jthread runner(&ThreadWorker::run, &worker, 200, 1, 20);
    char input;
    std::cin >> input;
    std::cout << "Done" << std::endl;
    runner.request_stop();
    return 0;
}
