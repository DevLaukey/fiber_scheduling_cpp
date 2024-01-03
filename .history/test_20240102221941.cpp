#include <iostream>
#include "Fiber.hpp"
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
    // Create an instance of the Scheduler
    Scheduler s;

    // Create fibers with func1 and func2
    Fiber f1(&func1);
    Fiber f2(&func2);

    // Spawn fibers into the scheduler
    s.spawn(&f1);
    s.spawn(&f2);

    // Run the scheduler
    s.do_it();

    return 0;
}
