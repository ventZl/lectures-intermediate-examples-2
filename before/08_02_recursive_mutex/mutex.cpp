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
        mut.lock();
        data.clear();
        mut.unlock();
    }

    std::string replace(const std::string & data) {
        mut.lock();
        auto tmp = this->data;
        set(data);
        mut.unlock();
        return tmp;
    }

    void set(const std::string & data) {
        mut.lock();
        this->data = data;
        mut.unlock();
    }

    std::string get() {
        mut.lock();
        auto tmp = this->data;
        mut.unlock();
        return tmp;
    }
protected:
    std::string data;
    std::mutex mut;
};

int main(int, char **) {
    DataBlock block;
    /* TODO: Below are three presumably thread-safe functions.
     * Each of them locks the mutex in order to make changes
     * to the object atomic.
     *
     * To save the amount of code written, these functions
     * call each other.
     *
     * Change the code in a way it won't deadlock when these functions
     * are called.
     */
    block.set("EHLO");
    std::cout << "Data is: " << block.get() << std::endl;
    block.clear();
    std::cout << "Data is: " << block.get() << std::endl;
    block.set("EHLO");
    block.replace("HELLO");
    std::cout << "Data is: " << block.get() << std::endl;
    return 0;
}
