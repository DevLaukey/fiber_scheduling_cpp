#include "context.hpp"

class Fiber
{
private:
    Context context;
    char *stack_bottom;
    char *stack_top;

public:
    Fiber(void (*function)())
    {
        // Allocate a new stack for the fiber
        const int stack_size = 4096; // Adjust the stack size as needed
        stack_bottom = new char[stack_size];
        stack_top = stack_bottom + stack_size;

        // Set up the context for the fiber
        context.rsp = stack_top; // Set stack pointer to the top of the stack
        context.rip = reinterpret_cast<void *>(function);

        // Initialize other registers to zero
        context.rbx = context.rbp = context.r12 = context.r13 = context.r14 = context.r15 = nullptr;
    }

    ~Fiber()
    {
        // Clean up any resources, e.g., deallocate the stack
        delete[] stack_bottom;
    }

};

void foo()
{
    std::cout << "Inside foo()" << std::endl;
}

int main()
{
    // Create a fiber with the foo function
    Fiber f(&foo);

    // Get the context of the fiber
    Context c = f.get_context();

    // Set the context
    f.set_context(c);

    // Switch to the fiber's context
    swap_context(&c, &f.get_context());

    std::cout << "Back to main()" << std::endl;

    return 0;
}
