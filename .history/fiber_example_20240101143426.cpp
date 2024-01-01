// fiber_example.cpp

#include <iostream>
#include <cstdint>

// Import the context functions from the assembly file
extern "C" int get_context(void *);
extern "C" void set_context(const void *);
extern "C" void swap_context(void *, const void *);

// Structure to represent the fiber's context
struct FiberContext
{
    uint8_t stack[4096]; // Fiber stack
    void *rip;           // Instruction pointer
    bool executedIf;     // Variable to indicate if 'if' statement has been executed
};

// Function representing the fiber body
void foo()
{
    std::cout << "You called foo" << std::endl;
}

int main()
{
    // Allocate space for the main fiber's context
    FiberContext mainContext;

    // Initialize mainContext
    mainContext.rip = nullptr;
    mainContext.executedIf = false;

    // Call get_context to save the initial state
    get_context(&mainContext);

    // Output a message
    std::cout << "A message" << std::endl;

    // Check the condition
    if (!mainContext.executedIf)
    {
        mainContext.executedIf = true;

        // Call set_context to restore the state and continue from here
        set_context(&mainContext);
    }

    // Extend the example to include a 'fiber' goo
    FiberContext gooContext;

    // Initialize gooContext
    gooContext.rip = nullptr;
    gooContext.executedIf = false;

    // Set the instruction pointer to the foo function
    gooContext.rip = (void *)foo;

    // Set the stack pointer to the top of the gooContext stack
    gooContext.rsp = gooContext.stack + sizeof(gooContext.stack);

    // Call swap_context to switch to the goo fiber
    swap_context(&mainContext, &gooContext);

    // The control will return here after the goo fiber completes
    std::cout << "Back to main" << std::endl;

    return 0;
}
