#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace std::chrono_literals;
std::exception_ptr exception = nullptr;

class ThreadWorker {
public:
    ThreadWorker(unsigned interval): m_interval{interval} {}

    void run()
    try
    {
        while (m_running) {
            std::cout << "Tick!" << m_running << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
        }
        /* Simulate unhandled exception thrown inside thread */
        throw std::runtime_error("Whooops!");
    } catch (...) {
        exception = std::current_exception();
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
    try {
        worker.stop();
        runner.join();
        if (exception) {
            std::rethrow_exception(exception);
        }
    } catch (std::exception & e) {
        std::cout << "Exception thrown: " << e.what() << std::endl;
    }
    return 0;
}
