#include <iostream>
#include <cstdint>
#include "context.hpp"

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
Fiber fooFiber(&foo);

void foo()
{
    std::cout << "You called foo" << std::endl;

    // Transfer control back to main using the saved context
    swap_context(&fooFiber.getContext(), &main_context);
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
    fooFiber.execute();

    // Control will be transferred to foo
    // Once foo completes, it will return to set_context, and set_context will set the context back to main
    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x)
    {
        std::cout << "Back in main after foo" << std::endl;
    }

    // Deallocate the main stack
    delete[] data_main;

    return 0;
}
