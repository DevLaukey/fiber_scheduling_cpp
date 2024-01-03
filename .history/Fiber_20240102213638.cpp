#include <iostream>
#include <cstdint>
#include "Fiber.hpp"
#include "context.hpp"

Fiber::Fiber(void (*func)(), int *dataPtr) : function(func), dataPtr(dataPtr) {}

int *Fiber::getDataPtr()
{
    return dataPtr;
}

void Fiber::execute()
{
    get_context(&context);
    function();
    // You can add additional cleanup or handling here if needed
}

Context Fiber::getContext()
{
    return context;
}

// Implement get_data based on your design
int *get_data()
{
    // Implement the logic to return the shared data pointer
    // This could be a pointer passed during fiber creation
    // or it can be a global/shared pointer based on your design
    return nullptr; // Replace with your implementation
}

// Your main and foo functions remain unchanged
volatile bool x = false;

Context main_context;

void foo()
{
    std::cout << "You called foo" << std::endl;

    // Get the shared data pointer
    int *dp = fooFiber.getDataPtr();

    // Your logic to manipulate shared data goes here

    // Store the context in a variable
    Context fooContext = fooFiber.getContext();

    // Transfer control back to main using the saved context
    swap_context(&fooContext, &main_context);
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

    // Create a fiber for foo with shared data
    int sharedData = 42; // Initialize shared data
    Fiber fooFiber(&foo, &sharedData);

    // Create another fiber or perform any additional setup as needed

    // Your logic to manipulate shared data goes here

    // Create a fiber for foo with shared data
    Fiber fooFiber(&foo, &sharedData);

    // Create another fiber or perform any additional setup as needed

    // Create a fiber for foo with shared data
    Fiber fooFiber(&foo, &sharedData);

    // Create another fiber or perform any additional setup as needed

    // Run your scheduler or fiber execution logic here

    // Deallocate the main stack
    delete[] data_main;

    return 0;
}
