#include <iostream>

#include "threadpool.h"

struct A {};

void foo(const A&) {}

int f(int a)
{
    return 5 * a - 3;
}

std::string f1(const std::string &s)
{
    return s + " c++";
}

int f2(int a, int b, int c)
{
    return a + b + c;
}

int main() {

    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A());
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    task2.get();

    auto task3 = pool.exec(f, 5);
    assert(task3.get() == 22);

    auto task4 = pool.exec(f1, "It's");
    assert(task4.get() == "It's c++");

    for (int i = 1; i <= 10; i++) {
        auto task = pool.exec(f2, i, 1, 1);
        assert(task.get() == i + 2);
    }

    return 0;
}
