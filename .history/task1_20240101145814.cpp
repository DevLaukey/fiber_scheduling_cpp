#include "context.hpp"
#include <iostream>

volatile int x = 0;

char fiber_stack[4096]; // Define a separate stack for the fiber

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
        // Set up the initial context for the fiber with a separate stack
        c.rsp = fiber_stack + sizeof(fiber_stack); // Set rsp to the top of the stack
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
