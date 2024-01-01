// fibers.cpp
#include <iostream>
#include "context.hpp"
#include <cstdint>

// Define stack size for each fiber
const int stack_size = 4096;

// Define data arrays for the stacks
char stack_foo[stack_size];
char stack_goo[stack_size];

// Function representing the first fiber's body
void foo()
{
    std::cout << "You called foo" << std::endl;

    // Transfer control to the goo function
    goo();
}

// Function representing the second fiber's body
void goo()
{
    std::cout << "You called goo" << std::endl;

    // Transfer control back to the foo function
    set_context(nullptr); // Passing nullptr to set_context restores the context saved previously

    // This line will not be reached as control is transferred to the foo function
    std::cerr << "Error: Control should not reach here" << std::endl;
}

int main()
{
    // Allocate space for the stack for foo
    uintptr_t *sp_foo = reinterpret_cast<uintptr_t *>(stack_foo + stack_size - 128);
    sp_foo = reinterpret_cast<uintptr_t *>((reinterpret_cast<uintptr_t>(sp_foo) & -16L));

    // Create an empty context c_foo for foo
    Context c_foo;
    c_foo.rip = (void *)&foo;
    c_foo.rsp = sp_foo;

    // Call set_context with c_foo
    set_context(&c_foo);

    // This line will not be reached as control is transferred to the foo function
    std::cerr << "Error: Control should not reach here" << std::endl;

    return 0;
}
