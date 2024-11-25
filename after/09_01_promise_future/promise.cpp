#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <future>

using namespace std::chrono_literals;

void file_loader(const std::string & file_name, std::promise<std::string> & content) {
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
    content.set_value(rv);
}

int main(int, char **) {
    std::string file_name = "input.txt";
    std::promise<std::string> promise;
    auto fut = promise.get_future();
    std::jthread runner(file_loader, std::ref(file_name), std::ref(promise));

    /* Do something interesting while waiting for file contents to be read */

    auto val = fut.get();
    std::cout << "File read: " << val << std::endl;
    return 0;
}
