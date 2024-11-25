#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

bool running = true;
const unsigned m_interval = 200;
char input;

int main(int, char **) {

    /* TODO: Make sure that the code below that simulates work being done
     * can be interrupted by keyboard input
     */
    while (running) {
        std::cout << "Tick!" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(m_interval));
    }

    /* TODO: This will never execute */
    std::cin >> input;
    std::cout << "Done" << std::endl;
    return 0;
}
