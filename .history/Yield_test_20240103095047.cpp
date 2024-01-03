#include <iostream>
#include "Scheduler.hpp"

void func1()
{
    std::cout << "fiber 1 before" << std::endl;
    Scheduler::get_instance().yield(); // Yield control to the scheduler
    std::cout << "fiber 1 after" << std::endl;
}

void func2()
{
    std::cout << "fiber 2" << std::endl;
    Scheduler::get_instance().fiber_exit();
}

int main()
{
    Scheduler s;

    // Create fibers with associated functions
    Fiber f1(func1, &s);
    Fiber f2(func2, &s);

    // Add fibers to the scheduler
    s.spawn(&f1);
    s.spawn(&f2);

    // Run the scheduler
    s.do_it();

    return 0;
}
