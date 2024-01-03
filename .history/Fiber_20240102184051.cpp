#include <iostream>
#include <cstdint>
#include "context.hpp"
#include "Fiber.hpp"

void foo(); // Forward declaration of the 'foo' function

// Fiber class definition
class Fiber
{
public:
    Fiber(void (*func)()) : function(func) {}

    void execute()
    {
        get_context(&context);
        function();
        // You can add additional cleanup or handling here if needed
    }

    Context getContext()
    {
        return context;
    }

private:
    Context context;
    void (*function)();
};

volatile bool x = false;

Context main_context;

void foo()
{
    std::cout << "You called foo" << std::endl;

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
    main_context.rip = reinterpret_cast<void *>(&foo); // Set to the address of foo
    main_context.rsp = sp_main;

    // Create a fiber for foo
    Fiber fooFiber(&foo);

    // Run the scheduler loop
    while (true)
    {
        fooFiber.execute(); // Execute the fiber

        // Due to the volatile keyword on x, the compiler will not optimize the condition
        if (x)
        {
            std::cout << "Back in main after foo" << std::endl;
            break; // Exit the loop after foo completes
        }
    }

    // Deallocate the main stack
    delete[] data_main;

    return 0;
}
