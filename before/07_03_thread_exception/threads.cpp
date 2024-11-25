#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace std::chrono_literals;

class ThreadWorker {
public:
    ThreadWorker(unsigned interval): m_interval{interval} {}

    void run()
    {
        while (m_running) {
            std::cout << "Tick!" << m_running << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }

        /* Simulate unhandled exception thrown inside thread */

        /* TODO: Handle this exception in a way that the current thread will
         * gracefully terminate and the exception will be handled by the
         * calling thread.
         */
        throw std::runtime_error("Whooops!");
    }

    void stop() {
        std::cout << "Not running" << std::endl;
        m_running = false;
    }

protected:
    volatile bool m_running = true;
    unsigned m_interval;
};

int main(int, char **) {
    ThreadWorker worker(5000);
    std::thread runner(&ThreadWorker::run, &worker);
    char input;
    std::cin >> input;
    std::cout << "Done" << std::endl;
    worker.stop();
    runner.join();
    return 0;
}
