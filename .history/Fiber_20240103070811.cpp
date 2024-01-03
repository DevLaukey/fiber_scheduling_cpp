// Fiber.cpp

#include <iostream>
#include <cstdint>
#include "context.hpp"
#include "Fiber.hpp"

volatile bool x = false; // Assume x is used in the code

Context main_context;

int *get_data(); // Assume get_data is used in the code

// Implementation of fiber_exit
void Fiber::fiber_exit()
{
    // Perform any necessary cleanup or handling here
    // For simplicity, we'll just return for now
    return;
}

Fiber::Fiber(void (*func)(), int *dataPtr) : function(func), dataPtr(dataPtr) {}

void Fiber::execute()
{
    get_context(&context);
    function();
    fiber_exit(); // Call fiber_exit after function execution
}

Context Fiber::getContext()
{
    return context;
}

int *Fiber::getDataPtr()
{
    return dataPtr;
}

int main()
{
    // Print start of function
    std::cout << "Start of main" << std::endl;

    // Allocate space for the main stack with additional 128 bytes for the Red Zone
    char *data_main = new char[4096 + 128];

    // Point the main stack pointer (sp_main) to the end of the allocated space
    char *sp_main = data_main + 4096 + 128;

    // Align the main stack to 16 bytes
    sp_main = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp_main) & -16L));

    // Adjust the main stack pointer for the Red Zone
    sp_main -= 128;

    // Set up main context
    get_context(&main_context);
    main_context.rip = nullptr; // Set to nullptr initially
    main_context.rsp = sp_main;

    // Create a fiber for foo
    Fiber fooFiber(&foo);

    // Transfer control to foo fiber
    fooFiber.execute();

    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x)
    {
        std::cout << "Back in main after foo" << std::endl;
    }

    // Deallocate the main stack
    delete[] data_main;

    return 0;
}
