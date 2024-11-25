#include <coroutine>
#include <iostream>

struct promise;
struct range : std::coroutine_handle<promise> { using promise_type = ::promise; };

struct promise {
    range get_return_object() { return {range::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    std::suspend_always yield_value(int i) { value = i; return {}; }
    void return_value(int i) { value = i; }
    void unhandled_exception() {}

    int value;
};

int main(int, char **) {
range r = [](int from, int to) -> range {
    while (from < to) { co_yield from++; } co_return from;
}(0, 100);

while (!r.done()) { r.resume(); std::cout << r.promise().value << std::endl; }
    return 0;
}
