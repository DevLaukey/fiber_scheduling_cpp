#include "Fiber.hpp"
#include <iostream>

// Constructor definitions
Fiber::Fiber() : stack_bottom(nullptr), stack_top(nullptr)
{
    context.rsp = context.rbx = context.rbp = context.r12 = context.r13 = context.r14 = context.r15 = nullptr;
    context.rip = nullptr;
}

Fiber::Fiber(void (*function)()) : Fiber()
{
    const int stack_size = 4096 + 128;
    stack_bottom = new char[stack_size];
    stack_top = stack_bottom + stack_size;
    stack_top = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(stack_top) & -16L));
    stack_top -= 128;

    context.rsp = stack_top;
    context.rip = reinterpret_cast<void *>(function);
}

// Destructor definition
Fiber::~Fiber()
{
    delete[] stack_bottom;
}

// Other member function definitions...

volatile bool x = false;

Fiber main_fiber;
Fiber goo_fiber;

void goo()
{
    std::cout << "You called goo" << std::endl;
}

void foo()
{
    std::cout << "You called foo" << std::endl;
}

int main()
{
    std::cout << "Start of main" << std::endl;

    // Create a fiber with the foo function
    Fiber foo_fiber(foo);

    // Get the context of the main fiber
    const Context &main_context = foo_fiber.get_context();

    // Call set_context with main context
    foo_fiber.set_context(main_context);

    // Control will be transferred to foo
    // Once foo completes, it will return to set_context, and set_context will set the context back to main
    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x)
    {
        std::cout << "Back in main after foo" << std::endl;
    }

    // Ensure foo_fiber is kept alive until this point
    // Deallocate resources (if needed)

    return 0;
}
