#include "context.hpp"
#include <iostream>
#include <cstdint>

volatile int x = 0;

char fiber_stack[4096]; // Separate stack for the fiber

void fiber_function()
{
    std::cout << "You called foo" << std::endl;
    set_context(nullptr);
}

int main()
{
    // Allocate space for the stack
    char data[4096];
    char *sp = data + sizeof(data);

    // Align the stack to 16 bytes
    sp = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp) & -16L));

    // Account for the Red Zone
    sp -= 128;

    // Set up the context for the fiber
    Context c;
    c.rsp = sp;
    c.rip = (void *)&fiber_function;

    // Save the current context (main context)
    get_context(&c);

    // Switch to the fiber's context
    set_context(&c);

    // This code will be executed after the fiber returns

    return 0;
}
