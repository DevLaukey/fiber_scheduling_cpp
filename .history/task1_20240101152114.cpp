#include "context.hpp"
#include <iostream>
#include <cstdint>

volatile int x = 0;

char fiber_function_stack[4096]; // Separate stack for fiber_function
char fiber_goo_stack[4096];      // Separate stack for fiber_goo

void fiber_function()
{
    std::cout << "You called fiber_function" << std::endl;
    set_context(nullptr); // Switch back to the previous context
}

void fiber_goo()
{
    std::cout << "You called fiber_goo" << std::endl;
    set_context(nullptr); // Switch back to the previous context
}

int main()
{
    // Allocate space for the stacks
    char data[4096];
    char *sp_function = data + sizeof(data);
    char *sp_goo = fiber_goo_stack + sizeof(fiber_goo_stack);

    // Align stacks to 16 bytes
    sp_function = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp_function) & -16L));
    sp_goo = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp_goo) & -16L));

    // Account for the Red Zone
    sp_function -= 128;
    sp_goo -= 128;

    // Set up the context for fiber_function
    Context c_function;
    c_function.rsp = sp_function;
    c_function.rip = (void *)&fiber_function;

    // Set up the context for fiber_goo
    Context c_goo;
    c_goo.rsp = sp_goo;
    c_goo.rip = (void *)&fiber_goo;

    for (int i = 0; i < 5; ++i)
    {
        // Switch to fiber_function
        set_context(&c_function);
        // Reset stack pointer
        sp_function = c_function.rsp;

        // Switch to fiber_goo
        set_context(&c_goo);
        // Reset stack pointer
        sp_goo = c_goo.rsp;
    }

    return 0;
}
