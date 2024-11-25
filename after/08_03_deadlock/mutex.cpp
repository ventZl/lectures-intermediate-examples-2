#include <thread>
#include <mutex>
#include <chrono>
#include <array>
#include <iostream>
#include <stdexcept>

using namespace std::chrono_literals;

struct Resource {
    long counter = 0;
    std::mutex mut;
};

struct BasicLockable {
    void lock() { mut.lock(); }
    void unlock() { mut.unlock(); }

protected:
    std::mutex mut;

};

class DiskDrive: public BasicLockable {
public:
    void use() {
        std::cout << "Accessing disk drive" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

class Terminal: public BasicLockable {
public:
    void use() {
        std::cout << "Accessing terminal" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};


class SerialLine: public BasicLockable {
public:
    void use() {
        std::cout << "Accessing serial line" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
};

/* Global resource instances */
DiskDrive disk;
Terminal terminal;
SerialLine serial;

/* Simulates loading data from serial and storing it to drive */
void save_serial_to_drive() {
    std::lock_guard disk_lock(disk);
    std::lock_guard serial_lock(serial);

    serial.use();


    disk.use();
}

/* Simulates loading data from disk and sending it to the serial */
void send_disk_data_to_serial() {
    std::lock_guard disk_lock(disk);

    disk.use();

    std::lock_guard serial_lock(serial);

    serial.use();
}

/* Simulates initiating disk access from terminal */
void terminal_action_to_access_disk() {
    std::lock_guard disk_lock(disk);
    std::lock_guard terminal_lock(terminal);

    terminal.use();


    disk.use();
}


int main(int, char **) {
    std::vector<std::thread> threads;
    threads.resize(3);
    threads[0] = std::thread(save_serial_to_drive);
    threads[1] = std::thread(send_disk_data_to_serial);
    threads[2] = std::thread(terminal_action_to_access_disk);

    for (auto & thread: threads) {
        thread.join();
    }

    return 0;
}
