#include <iostream>
#include "context.hpp"

// Define the stack size for the fiber
const int stack_size = 4096;

// Function representing the fiber's body
void foo()
{
    std::cout << "You called foo" << std::endl;
    // Cannot return to main as they have different stacks
    exit(0);
}

int main()
{
    // Allocate space for the stack
    char data[stack_size];

    // Set the stack pointer to the end of the allocated space
    uintptr_t *sp = reinterpret_cast<uintptr_t *>(data + stack_size);

    // Align the stack pointer according to Sys V ABI
    *sp &= -16L;

    // Adjust for Red Zone
    *sp -= 128;

    // Create an empty context c for foo
    Context c;
    c.rip = reinterpret_cast<void *>(&foo);
    c.rsp = reinterpret_cast<void *>(sp);

    // Call set_context with c to switch to the fiber
    set_context(&c);

    // This line will not be reached as control is transferred to the foo function
    std::cerr << "Error: Control should not reach here" << std::endl;

    return 0;
}
