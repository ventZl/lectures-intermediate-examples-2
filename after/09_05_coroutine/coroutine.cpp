#include <coroutine>
#include <iostream>
#include <vector>

struct promise;
struct range : std::coroutine_handle<promise> { using promise_type = ::promise; };

std::vector<range> tasks;
int current_task = 0;

struct promise {
    using handle_type = std::coroutine_handle<promise>;
    struct task_switcher {
        task_switcher(handle_type & next) : next_coro{ next } {}
        bool await_ready() { return false; }
        auto await_suspend(std::coroutine_handle<>) { return next_coro; }
        void await_resume() {}

        handle_type & next_coro;
    };
    range get_return_object() { return {range::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept {
        return {};
    }
    task_switcher yield_value(int) {
        current_task = (current_task + 1) % tasks.size();
        return { tasks[current_task] };
    }
    void return_void() { }
    void unhandled_exception() {}

    int value;
};

int main(int, char **) {
    tasks.emplace_back([]() -> range {
        while (true) { std::cout << "First!" << std::endl; co_yield{}; } co_return;
    }());

    tasks.emplace_back([]() -> range {
        while (true) { std::cout << "Second!" << std::endl; co_yield{}; } co_return;
        }());

    tasks[0].resume();
    return 0;
}
