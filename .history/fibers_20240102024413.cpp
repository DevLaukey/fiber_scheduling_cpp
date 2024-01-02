#include <iostream>
#include "context.hpp"

class Fiber
{
public:
    Fiber(void (*func)()) : function(func), context()
    {
        // Allocate stack space for the fiber
        stack = new char[stack_size];

        // Align the stack pointer according to Sys V ABI
        uintptr_t sp = reinterpret_cast<uintptr_t>(stack + stack_size);
        sp &= -16L;
        sp -= 128; // Adjust for Red Zone

        // Set the stack pointer and instruction pointer in the context
        context.rsp = reinterpret_cast<void *>(sp);
        context.rip = reinterpret_cast<void *>(func);
    }

    ~Fiber()
    {
        delete[] stack;
    }

    Context getContext() const
    {
        return context;
    }

    void switchTo()
    {
        Context currentContext;
        get_context(&currentContext);

        // Save the current context
        set_context(&context);

        // When control returns here, the context is restored
        get_context(&context);

        // Return to the original context
        set_context(&currentContext);
    }

private:
    static const int stack_size = 4096;
    char *stack;
    void (*function)();
    Context context;
};

volatile int x = 0;

void foo()
{
    std::cout << "You called foo" << std::endl;
}

int main()
{
    char data[4096];
    uintptr_t *sp = reinterpret_cast<uintptr_t *>(data + sizeof(data));

    // Align the stack pointer according to Sys V ABI
    *sp &= -16L;

    // Adjust for Red Zone
    *sp -= 128;

    // Create an empty context c for foo
    Context c;
    c.rip = reinterpret_cast<void *>(&foo);
    c.rsp = reinterpret_cast<void *>(sp);

    // Create a fiber with the foo function and switch to it
    Fiber fiber(&foo);
    fiber.switchTo();

    return 0;
}
