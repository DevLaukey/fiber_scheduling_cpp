#pragma once

#include "context.hpp"
#include <cstdint>
#include <functional>

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

    Fiber(std::function<void()> function) : Fiber()
    {
        const int stack_size = 4096 + 128;
        stack_bottom = new char[stack_size];
        stack_top = stack_bottom + stack_size;
        stack_top = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(stack_top) & -16L));
        stack_top -= 128;

        context.rsp = stack_top;
        context.rip = reinterpret_cast<void *>(function.target<void()>());
    }

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

    void start_execution(Fiber &other)
    {
        swap_context(other);
    }

    Context &get_context()
    {
        return context;
    }
};
