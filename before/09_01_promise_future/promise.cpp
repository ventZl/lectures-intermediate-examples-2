#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using namespace std::chrono_literals;

void file_loader(const std::string & file_name, std::string & content) {
    std::ifstream file;
    std::string rv;
    char buf[1024];

    file.open(file_name, std::fstream::in);

    if (file.is_open()) {
        while (!file.eof()) {
            file.read(buf, sizeof(buf));
            rv += buf;
        }
    }
    content = rv;
}

int main(int, char **) {
    std::string file_name = "input.txt";
    std::string val;
    std::thread runner(file_loader, std::ref(file_name), std::ref(val));

    /* TODO: Fix this code so that the main thread can do something else
     * than just wait for the worker thread. Main thread will then synchronize
     * with worker thread using promise and future.
     */

    runner.join();

    std::cout << "File read: " << val << std::endl;
    return 0;
}
