#include "Fiber.hpp"
#include <iostream>

// Constructor definitions
Fiber::Fiber() : stack_bottom(nullptr), stack_top(nullptr)
{
    context.rsp = context.rbx = context.rbp = context.r12 = context.r13 = context.r14 = context.r15 = nullptr;
    context.rip = nullptr;
}

Fiber::Fiber(void (*function)()) : Fiber()
{
    const int stack_size = 4096 + 128;
    stack_bottom = new char[stack_size];
    stack_top = stack_bottom + stack_size;
    stack_top = align_stack(stack_top);
    stack_top -= 128;

    context.rsp = stack_top;
    context.rip = reinterpret_cast<void *>(function);
}

// Destructor definition
Fiber::~Fiber()
{
    delete[] stack_bottom;
}

// Member function definitions

Context *get_context()
{
    return &context;
}

void Fiber::set_context(const Context &newContext)
{
    context = newContext;
}

void Fiber::swap_context(Fiber &other)
{
    std::swap(context, other.context);
}

void Fiber::start_execution(Fiber &other)
{
    swap_context(other);
}

// Helper function to align the stack
char *Fiber::align_stack(char *stackPointer)
{
    return reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(stackPointer) & -16L));
}
