#include <iostream>
#include <cstdint>
#include "context.hpp"

volatile bool x = false;

Context main_context;
Context goo_context;

void goo(); // Forward declaration

void foo()
{
    std::cout << "You called foo" << std::endl;

    // Create a context for goo
    get_context(&goo_context);
    goo_context.rip = (void *)&goo; // Set rip to goo, so control will switch to goo
    set_context(&goo_context);      // Transfer control to goo
}

void goo()
{
    std::cout << "You called goo" << std::endl;

    // Switch back to foo
    swap_context(&goo_context, &main_context);
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
    main_context.rip = (void *)&foo;
    main_context.rsp = sp_main;

    // Call set_context with main context
    set_context(&main_context);

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
