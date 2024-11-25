#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <future>
#include <stdexcept>

using namespace std::chrono_literals;

std::string file_loader(const std::string & file_name) {
    std::ifstream file;
    std::string rv;
    char buf[1024];

    file.open(file_name, std::fstream::in);

    if (file.is_open()) {
        while (!file.eof()) {
            file.read(buf, sizeof(buf));
            rv += buf;
        }
    } else {
        throw std::runtime_error("File not found!");
    }

    return rv;
}

int main(int, char **) {
    std::string file_name = "input.txt";

    auto fut = std::async(std::launch::async, file_loader, std::ref(file_name));

    /* Do something interesting while waiting for file contents to be read */

    try {
        auto val = fut.get();
        std::cout << "File read: " << val << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
