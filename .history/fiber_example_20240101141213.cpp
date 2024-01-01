// fiber_example.cpp

#include "context.hpp"
#include <iostream>
#include <cstdint>

// Define the Context struct
struct Context
{
    void *rip, *rsp;
    void *rbx, *rbp, *r12, *r13, *r14, *r15;
};

// Function representing the fiber's body
void foo()
{
    std::cout << "You called foo" << std::endl;
    // Cannot return to main, so exit the fiber
    exit(0);
}

int main()
{
    // Allocate space for stack
    char data[4096];

    // Align the stack to 16 bytes
    uintptr_t sp = reinterpret_cast<uintptr_t>(data + 4096);
    sp &= -16L;

    // Account for the Red Zone
    sp -= 128;

    // Create an empty context c for foo
    // Change the function signature of set_context to accept a pointer to Context
    void set_context(Context* context);

    Context c;
    c.rip = reinterpret_cast<void *>(&foo);
    c.rsp = reinterpret_cast<void *>(sp);

    // Call set_context with &c to save the initial context
    set_context(&c);

    // Variable indicating if the if block has been executed
    volatile int x = 0;

    // Save the state at line 3
    Context savedContext;
    get_context(&savedContext);

    std::cout << "A message" << std::endl;

    // If x == 0, execute the if's body
    if (x == 0)
    {
        x += 1;

        // Call set_context with the saved context to return to line 4
        set_context(&savedContext);
    }

    std::cout << "A message" << std::endl;

    return 0;
}
