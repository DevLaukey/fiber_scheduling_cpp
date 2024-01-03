
#include <iostream>
#include <cstdint>
#include "context.hpp"
#include "Fiber.hpp"


class fiber
{
private:
    char *stack_bottom;
    char *stack_top;

public:
    fiber(void (*function)())
    {
        // Initialize stack_bottom and stack_top based on the requirements
        // This could involve allocating a new stack for the fiber
        // and setting stack_bottom and stack_top accordingly.

        // For simplicity, let's assume some stack allocation here
        const int stack_size = 4096; // Adjust the stack size as needed
        stack_bottom = new char[stack_size];
        stack_top = stack_bottom + stack_size;

        // Create the context and set its stack
        Context::set_stack(stack_bottom, stack_top);

        // Set up the initial context for the fiber, pointing to the provided function
        Context::make_context(function);
    }

    ~fiber()
    {
        // Clean up any resources, e.g., deallocate the stack
        delete[] stack_bottom;
    }

    Context get_context()
    {
        // Return the context of the fiber
        return Context::get_current_context();
    }
};

// Example function
void foo()
{
    std::cout << "Inside foo()" << std::endl;
}

int main()
{
    // Create a fiber with the foo function
    fiber f(&foo);

    // Get the context of the fiber
    Context c = f.get_context();

    // Set the context
    Context::set_context(c);

    // Switch to the fiber's context
    Context::swap_context();

    std::cout << "Back to main()" << std::endl;

    return 0;
}
