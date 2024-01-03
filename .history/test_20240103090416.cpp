#include <iostream>
#include "Scheduler.hpp"
#include "Fiber.hpp"

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

    Fiber f1([&s]()
             { func1(s); }); // Pass Scheduler reference to lambda
    Fiber f2([&s]()
             { func2(s); }); // Pass Scheduler reference to lambda

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}
