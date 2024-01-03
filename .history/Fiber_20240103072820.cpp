#include <iostream>
#include <cstdint>
#include "context.hpp"

class Fiber
{
private:
    Context context;
    char *stack_bottom;
    char *stack_top;

public:
    // ... (unchanged)

    // Function to start executing the fiber
    void start_execution()
    {
        swap_context(main_fiber, *this);
    }
};

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
    // Print start of function
    std::cout << "Start of main" << std::endl;

    // Create a fiber with the foo function
    Fiber foo_fiber(foo);

    // Get the context of the main fiber
    const Context &main_context = foo_fiber.get_context();

    // Call set_context with main context
    foo_fiber.set_context(main_context);

    // Start the execution of foo_fiber
    foo_fiber.start_execution();

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
