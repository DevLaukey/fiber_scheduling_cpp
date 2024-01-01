// fibers.cpp
#include <iostream>
#include "context.hpp"
#include <cstdint>

// Define a stack size
const int stack_size = 4096;

// Define a data array for the stack
char data[stack_size];

// Function representing the fiber's body
void foo()
{
    std::cout << "You called foo" << std::endl;

    // Transfer control to the exit function
    exit(0);
}

int main()
{
    // Allocate space for the stack
    uintptr_t *sp = reinterpret_cast<uintptr_t *>(data + stack_size);

    // Ensure stack alignment to 16 bytes
    sp = reinterpret_cast<uintptr_t *>((reinterpret_cast<uintptr_t>(sp) & -16L));

    // Account for the Red Zone
    sp = reinterpret_cast<uintptr_t *>(reinterpret_cast<char *>(sp) - 128);

    // Create an empty context c
    Context c;

    // Set rip and rsp of c
    c.rip = (void *)&foo;
    c.rsp = sp;

    // Call set_context with c
    set_context(&c);

    // This line will not be reached as control is transferred to the foo function
    std::cerr << "Error: Control should not reach here" << std::endl;

    return 0;
}
