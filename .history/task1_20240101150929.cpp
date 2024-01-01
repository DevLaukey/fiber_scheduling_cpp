#include "context.hpp"

void myFiberFunction()
{
    // Your fiber's code here
    set_context(nullptr); // Return to the previous context
}

int main()
{
    // Set up initial context for the fiber
    Context fiberContext;
    fiberContext.rsp = nullptr;
    fiberContext.rip = (void *)&myFiberFunction;

    // Save the current context (main context)
    get_context(&fiberContext);

    // Switch to the fiber's context
    set_context(&fiberContext);

    // This code will be executed after the fiber returns

    return 0;
}
