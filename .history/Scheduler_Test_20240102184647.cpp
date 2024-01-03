#include <iostream>
#include "context.hpp"
#include "Fiber.hpp"
#include "Scheduler.hpp"

void func1()
{
    std::cout << "fiber 1" << std::endl;
}

void func2()
{
    std::cout << "fiber 2" << std::endl;
}

void func3()
{
    std::cout << "fiber 3" << std::endl;
}

int main()
{
    Scheduler s;

    // Creating fibers
    Fiber f1(&func1, &s);
    Fiber f2(&func2, &s);
    Fiber f3(&func3, &s);

    // Spawning fibers
    s.spawn(&f1);
    s.spawn(&f2);
    s.spawn(&f3);

    // Running the scheduler
    s.do_it();

    return 0;
}
