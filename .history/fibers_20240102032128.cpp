#include "context.hpp"
#include <iostream>

volatile bool x = false;

extern "C" void set_context(Context *c);

void foo()
{
    std::cout << "You called foo" << std::endl;
    x = true;
    set_context(nullptr);
}

int main()
{
    // Allocate space for the stack
    char *data = new char[4096];

    // Point the stack pointer (sp) to the end of the allocated space
    char *sp = data + 4096;

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
