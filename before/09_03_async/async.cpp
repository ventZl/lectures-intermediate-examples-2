#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <future>
#include <stdexcept>

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
    } else {
        throw std::runtime_error("File not found!");
    }
    content.set_value(rv);
}

int main(int, char **) {
    std::string file_name = "input.txt";

    std::promise<std::string> promise;
    auto fut = promise.get_future();
    std::jthread runner(file_loader, std::ref(file_name), std::ref(promise));

    /* TODO: Rewrite this piece of code using std::async */

    try {
        auto val = fut.get();
        std::cout << "File read: " << val << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
