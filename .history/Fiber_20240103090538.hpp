#pragma once
#include "context.hpp"
#include <functional>

class Fiber
{
private:
    Context context;
    char *stack_bottom;
    char *stack_top;

public:
    Fiber(std::function<void()> function);
    ~Fiber();
    const Context &get_context() const;
    void set_context(const Context &newContext);
    void swap_context(Fiber &other);
    void start_execution(Fiber &other);
};
