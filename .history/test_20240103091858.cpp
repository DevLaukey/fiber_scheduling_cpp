#include <iostream>
#include "Scheduler.hpp"
#include <functional> // Add this include for std::function

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

    // Use a lambda with explicit type for Fiber construction
    auto lambda1 = [&s]()
    { func1(s); };
    auto lambda2 = [&s]()
    { func2(s); };

    Fiber f1(static_cast<std::function<void()>>(lambda1)); // Cast the lambda to std::function
    Fiber f2(static_cast<std::function<void()>>(lambda2)); // Cast the lambda to std::function

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}
