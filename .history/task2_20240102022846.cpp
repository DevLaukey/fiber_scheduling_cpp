#include <iostream>
#include "context.hpp"

class Fiber
{
public:
    // Constructor for creating a fiber with a given function
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

    // Destructor to release allocated stack space
    ~Fiber()
    {
        delete[] stack;
    }

    // Get the context of the fiber
    Context getContext() const
    {
        return context;
    }

private:
    static const int stack_size = 4096;
    char *stack;
    void (*function)(); // Function to be executed by the fiber
    Context context;    // Context of the fiber
};

int main()
{
    // Create a fiber with the foo function
    Fiber f([]()
            { std::cout << "You called foo" << std::endl; });

    // Get the context of the fiber
    Context c = f.getContext();

    // Set the context using set_context
    set_context(&c);

    return 0;
}
