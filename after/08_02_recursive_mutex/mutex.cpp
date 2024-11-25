#include <thread>
#include <mutex>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std::chrono_literals;

class DataBlock {
public:
    void clear() {
        std::lock_guard lock(mut);
        data.clear();
    }

    std::string replace(const std::string & data) {
        std::lock_guard lock(mut);
        auto tmp = this->data;
        set(data);
        return tmp;
    }

    void set(const std::string & data) {
        std::lock_guard lock(mut);
        this->data = data;
    }

    std::string get() {
        std::lock_guard lock(mut);
        auto tmp = this->data;
        return tmp;
    }
protected:
    std::string data;
    std::recursive_mutex mut;
};

int main(int, char **) {
    DataBlock block;
    block.set("EHLO");
    std::cout << "Data is: " << block.get() << std::endl;
    block.clear();
    std::cout << "Data is: " << block.get() << std::endl;
    block.set("EHLO");
    block.replace("HELLO");
    std::cout << "Data is: " << block.get() << std::endl;
    return 0;
}
