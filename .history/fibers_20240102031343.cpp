#include "context.hpp"
#include <iostream>

volatile bool x = false;

extern "C" void set_context(Context *c);

void foo()
{
    std::cout << "You called foo" << std::endl;
    x = true;             // Mark x as volatile to avoid optimization
    set_context(nullptr); // Transfer control to the context set by main
}

int main()
{
    // Allocate space for stack
    char data[4096];

    // Stacks grow downwards
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

    return 0;
}
