#include <iostream>
#include "Scheduler.hpp"

int *get_data(Fiber &f)
{
    return f.get_data_pointer();
}

void func1(Scheduler &s)
{
    std::cout << "fiber 1" << std::endl;
    int *dp = get_data(s.current_fiber());
    if (dp)
    {
        std::cout << "fiber 1: " << *dp << std::endl;
        (*dp)++;
    }
    s.fiber_exit();
}

void func2(Scheduler &s)
{
    int *dp = get_data(s.current_fiber());
    if (dp)
    {
        std::cout << "fiber 2: " << *dp << std::endl;
    }
    s.fiber_exit();
}

int main()
{
    Scheduler s;

    int d = 10;
    int *dp = &d;

    Fiber f1([&s]()
             { func1(s); },
             dp);
    Fiber f2([&s]()
             { func2(s); },
             dp);

    s.spawn(&f1);
    s.spawn(&f2);

    s.do_it();

    return 0;
}
