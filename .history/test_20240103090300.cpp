#include <iostream>
#include "Scheduler.hpp"

void func1(Scheduler &s)
{
    std::cout << "fiber 1" << std::endl;
    s.fiber_exit(); // Simplified fiber exit within the test code
}

void func2(Scheduler &s)
{
    std::cout << "fiber 2" << std::endl;
    s.fiber_exit(); // Simplified fiber exit within the test code
}

int main()
{
    Scheduler s;

    Fiber f1([&]()
             { func1(s); });
    Fiber f2([&]()
             { func2(s); });

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}
