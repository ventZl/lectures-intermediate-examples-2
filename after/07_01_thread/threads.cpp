#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

class ThreadWorker {
public:
    ThreadWorker(unsigned interval): m_interval{interval} {}

    void run() {
        while (running) {
            std::cout << "Tick!" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }
    }

    void stop() {
        running = false;
    }

protected:
    bool running = true;
    unsigned m_interval;
};

int main(int, char **) {
    ThreadWorker worker(1000);
    std::thread runner(&ThreadWorker::run, &worker);
    char input;
    std::cin >> input;
    std::cout << "Done" << std::endl;
    worker.stop();
    return 0;
}
