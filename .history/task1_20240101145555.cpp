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
        // Set up the initial context for the fiber
        c.rsp = nullptr; // Set rsp to null for the first setup
        c.rip = (void *)&fiber;
        set_context(&c);

        // Check if the fiber executed and reset x
        if (x == 1)
        {
            x = 0;
        }
    }

    return 0;
}
