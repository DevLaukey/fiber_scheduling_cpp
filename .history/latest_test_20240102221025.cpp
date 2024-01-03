// main.cpp

#include <iostream>
#include "context.hpp"
#include "Fiber.hpp"
#include "Scheduler.hpp"

// Function for fiber 1
void func1()
{
    std::cout << "fiber 1" << std::endl;
    int *dp = get_data();
    std::cout << "fiber 1: " << *dp << std::endl;
    (*dp)++;
}

// Function for fiber 2
void func2()
{
    int *dp = get_data();
    std::cout << "fiber 2: " << *dp << std::endl;
}

// Global scheduler instance
Scheduler s;

int main()
{
    int d = 10;
    int *dp = &d;

    // Creating fibers
    Fiber f1(&func1, dp);
    Fiber f2(&func2, dp);

    // Spawning fibers
    s.spawn(&f1);
    s.spawn(&f2);

    // Running the scheduler
    s.do_it();

    return 0;
}
