#include <iostream>
#include "context.hpp"

volatile int x = 0;

void fiber()
{
    Context c;

    // Save the current context
    get_context(&c);

    std::cout << "a message" << std::endl;

    if (x == 0)
    {
        x++;
        // Restore the saved context
        set_context(&c);
    }
}

int main()
{
    fiber(); // First execution of fiber

    fiber(); // Second execution of fiber

    return 0;
}
