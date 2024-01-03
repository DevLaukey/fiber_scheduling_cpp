#include <iostream>
#include "Scheduler.hpp"

void func1()
{
    std::cout << "fiber 1" << std::endl;
    fiber_exit();
}

void func2()
{
    std::cout << "fiber 2" << std::endl;
    fiber_exit();
}

int main()
{
    Scheduler s;

    Fiber f1(func1);
    Fiber f2(func2);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}
