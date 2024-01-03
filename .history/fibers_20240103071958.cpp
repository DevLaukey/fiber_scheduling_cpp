#include <iostream>
#include <cstdint>
#include "context.hpp"

class Fiber {
private:
    Context context;
    char* stack_bottom;
    char* stack_top;

public:
    Fiber(void (*function)()) {
        // Allocate a new stack for the fiber
        const int stack_size = 4096 + 128; // Adjust the stack size as needed
        stack_bottom = new char[stack_size];
        stack_top = stack_bottom + stack_size;

        // Align the stack to 16 bytes
        stack_top = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(stack_top) & -16L));

        // Adjust the stack pointer for the Red Zone
        stack_top -= 128;

        // Set up the context for the fiber
        context.rsp = stack_top;  // Set stack pointer to the top of the stack
        context.rip = reinterpret_cast<void*>(function);

        // Initialize other registers to zero
        context.rbx = context.rbp = context.r12 = context.r13 = context.r14 = context.r15 = nullptr;
    }

    ~Fiber() {
        // Clean up any resources, e.g., deallocate the stack
        delete[] stack_bottom;
    }

    Context get_context() {
        // Get the context of the fiber
        return context;
    }

    void set_context(Context newContext) {
        // Set the context of the fiber
        context = newContext;
    }

    void swap_context(Fiber& other) {
        // Swap the context between two fibers
        Context currentContext;
        get_context(&currentContext);  // Get the current context

        set_context(other.context);    // Set the other fiber's context
        swap_context(&other.context, &currentContext);  // Swap the contexts

        // After the swap, control will return here
        // Update the current fiber's context
        set_context(currentContext);
    }
};

volatile bool x = false;

Fiber main_fiber;
Fiber goo_fiber;

void goo() {
    std::cout << "You called goo" << std::endl;
}

int main() {
    // Print start of function
    std::cout << "Start of main" << std::endl;

    // Create a fiber with the foo function
    Fiber foo_fiber(&foo);

    // Get the context of the main fiber
    Context main_context = foo_fiber.get_context();

    // Call set_context with main context
    foo_fiber.set_context(main_context);

    // Control will be transferred to foo
    // Once foo completes, it will return to set_context, and set_context will set the context back to main
    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x) {
        std::cout << "Back in main after foo" << std::endl;
    }

    // Deallocate resources (if needed)

    return 0;
}
