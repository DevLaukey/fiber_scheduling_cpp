#pragma once

#include "context.hpp"
#include <cstdint>
#include <functional>

class Scheduler;

class Fiber
{
private:
    Context context;
    char *stack_bottom;
    char *stack_top;
    int *sharedDataPointer;
    Scheduler *scheduler;

public:
    Fiber();

    Fiber(std::function<void()> function);

    ~Fiber();

    const Context &get_context() const;

    void set_context(const Context &newContext);

    void swap_context(Fiber &other);

    void start_execution(Fiber &other);

    Context &get_context();

    int *get_data_pointer() const;

    void yield();
};
