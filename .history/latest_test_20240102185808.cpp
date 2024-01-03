#include <iostream>
#include "context.hpp"
#include "Fiber.hpp"
#include "Scheduler.hpp"

void func1()
{
    std::cout << "fiber 1" << std::endl;

    int *dp = get_data();
    if (dp)
    {
        std::cout << "fiber 1: " << *dp << std::endl;
        (*dp)++;
    }

    fiber_exit();
}

void func2()
{
    int *dp = get_data();
    if (dp)
    {
        std::cout << "fiber 2: " << *dp << std::endl;
    }

    fiber_exit();
}

int main()
{
    Scheduler s;

    int d = 10;
    int *dp = &d;

    Fiber f1(&func1, dp);
    Fiber f2(&func2, dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}
