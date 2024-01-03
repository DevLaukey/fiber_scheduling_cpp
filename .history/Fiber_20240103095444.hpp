#pragma once

#include "context.hpp"
#include <cstdint>

class Scheduler; // Forward declaration of Scheduler class

class Fiber
{
private:
    Context context;
    char *stack_bottom;
    char *stack_top;

public:
    Fiber() : stack_bottom(nullptr), stack_top(nullptr)
    {
        context.rsp = context.rbx = context.rbp = context.r12 = context.r13 = context.r14 = context.r15 = nullptr;
        context.rip = nullptr;
    }

    Fiber(void (*function)(), Scheduler *scheduler); // Update the constructor

    ~Fiber()
    {
        delete[] stack_bottom;
    }

    const Context &get_context() const
    {
        return context;
    }

    void set_context(const Context &newContext)
    {
        context = newContext;
    }

    void swap_context(Fiber &other)
    {
        std::swap(context, other.context);
    }

    void start_execution(Scheduler &scheduler); // Update the method
};
