#include "context.hpp"
#include <iostream>

volatile int x = 0;

void fiber()
{
    std::cout << "a message" << std::endl;
    if (x == 0)
    {
        x++;
        set_context(nullptr);
    }
}

int main()
{
    Context c;

    for (;;)
    {
        if (x == 1)
        {
            // The fiber has executed, reset x and set up the fiber again
            x = 0;
            c.rsp = nullptr; // Set rsp to null to indicate a new fiber setup
            c.rip = (void *)&fiber;
            set_context(&c);
        }
        else
        {
            // This is the first time or the fiber has not executed yet
            c.rsp = nullptr; // Set rsp to null for the first setup
            c.rip = (void *)&fiber;
            set_context(&c);
        }
    }

    return 0;
}
