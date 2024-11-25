#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

class ThreadWorker {
public:
    void run(int interval, int from, int to) {
        /* TODO: Modify this code so the execution can be cancelled from the
         * calling thread if keyboard input from the user is present before
         * the routine is done.
         */
        while (from < to) {
            std::cout << from++ << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }
};

int main(int, char **) {
    ThreadWorker worker;
    std::thread runner(&ThreadWorker::run, &worker, 200, 1, 20);
    char input;
    std::cin >> input;
    std::cout << "Done" << std::endl;
    runner.join();
    return 0;
}
