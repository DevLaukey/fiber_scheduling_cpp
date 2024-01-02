#include <iostream>
#include <cstdint>
#include "context.hpp"

volatile bool x = false;

Context main_context;
int main();

void goo()
{
    std::cout << "You called goo" << std::endl;

    // Save the current context
    Context current_context;
    get_context(&current_context);

    // Update the main_context
    main_context = current_context;

    // Transfer control back to the main context
    swap_context(&main_context, nullptr);
}

void foo()
{
    std::cout << "You called foo" << std::endl;

    // Save the current context
    Context current_context;
    get_context(&current_context);

    current_context.rip = (void *)&main;

    // print the context
    std::cout << "rip: " << current_context.rip << std::endl;

    x=true;

    swap_context(&main_context, &current_context); // Transfer control back to main context

}

int main()
{
    // print in main
    std::cout << "You called main" << std::endl;
    // Once foo completes, it will return to set_context, and set_context will set the context back to main
    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x)
    {
        std::cout << "Back in main after foo" << std::endl;
    }


    // Allocate space for the stack with additional 128 bytes for the Red Zone
    char *data_main = new char[4096 + 128];

    // Point the stack pointer (sp_main) to the end of the allocated space
    char *sp_main = data_main + 4096 + 128;

    // Align the stack to 16 bytes
    sp_main = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp_main) & -16L));

    // Adjust the stack pointer for the Red Zone
    sp_main -= 128;

    // Set up main context
    get_context(&main_context);
    main_context.rip = (void *)&foo;
    main_context.rsp = sp_main;

    // Call set_context with main context


    // Deallocate the main stack
    delete[] data_main;

    // Now, let's create a new fiber for goo

    // Allocate space for the stack with additional 128 bytes for the Red Zone
    char *data_goo = new char[4096 + 128];

    // Point the stack pointer (sp_goo) to the end of the allocated space
    char *sp_goo = data_goo + 4096 + 128;

    // Align the stack to 16 bytes
    sp_goo = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp_goo) & -16L));

    // Adjust the stack pointer for the Red Zone
    sp_goo -= 128;

    // Create an empty context c_goo
    Context c_goo;

    // Set rip of c_goo to goo
    c_goo.rip = (void *)&goo;

    // Set rsp of c_goo to sp_goo
    c_goo.rsp = sp_goo;

    // Call swap_context to switch from main to goo
    swap_context(&main_context, &c_goo);

    // Control will be transferred to goo
    // Once goo completes, it will return to set_context, and set_context will set the context back to main
    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x)
    {
        std::cout << "Back in main after goo" << std::endl;
    }

    // Deallocate the goo stack
    delete[] data_goo;

    return 0;
}
