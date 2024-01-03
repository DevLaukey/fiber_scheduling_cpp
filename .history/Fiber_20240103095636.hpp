#include "Fiber.hpp"
#include "Scheduler.hpp"

Fiber::Fiber() : stack_bottom(nullptr), stack_top(nullptr), sharedDataPointer(nullptr), scheduler(nullptr)
{
    context.rsp = context.rbx = context.rbp = context.r12 = context.r13 = context.r14 = context.r15 = nullptr;
    context.rip = nullptr;
}

Fiber::Fiber(std::function<void()> function) : Fiber()
{
    const int stack_size = 4096 + 128;
    stack_bottom = new char[stack_size];
    stack_top = stack_bottom + stack_size;
    stack_top = reinterpret_cast<char *>((reinterpret_cast<uintptr_t>(stack_top) & -16L));
    stack_top -= 128;

    context.rsp = stack_top;
    context.rip = reinterpret_cast<void *>(function.target<void()>());
}

Fiber::~Fiber()
{
    delete[] stack_bottom;
}

const Context &Fiber::get_context() const
{
    return context;
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

Context &Fiber::get_context()
{
    return context;
}

int *Fiber::get_data_pointer() const
{
    return sharedDataPointer;
}

void Fiber::yield()
{
    if (scheduler)
    {
        // Save the current fiber's context
        set_context(context);
        // Resume the scheduler
        scheduler->do_it();
    }
}
