#include <iostream>
#include <cstdint>
#include "context.hpp"

volatile bool x = false;


void foo()
{
    std::cout << "You called foo" << std::endl;
    x = true;
    set_context(nullptr);
}

int main()
{
    //print start of funciton
    std::cout << "Start of main" << std::endl;
    // Allocate space for the stack with additional 128 bytes for the Red Zone
    char *data = new char[4096 + 128];

    // Point the stack pointer (sp) to the end of the allocated space
    char *sp = data + 4096 + 128;

    // Align the stack to 16 bytes
    sp = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(sp) & -16L));

    // Adjust the stack pointer for the Red Zone
    sp -= 128;

    // Create an empty context c
    Context c;

    // Set rip of c to foo
    c.rip = (void *)&foo;

    // Set rsp of c to sp
    c.rsp = sp;

    // Call set_context with c
    set_context(&c);

    // Control will be transferred to foo
    // Once foo completes, it will return to set_context, and set_context will set the context back to main
    // Due to the volatile keyword on x, the compiler will not optimize the condition
    if (x)
    {
        std::cout << "Back in main" << std::endl;
    }

    // Deallocate the stack
    delete[] data;

    return 0;
}
