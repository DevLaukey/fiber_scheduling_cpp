#ifndef FIBER_HPP
#define FIBER_HPP

#include "context.hpp"

class Fiber
{
private:
    Context context;
    char *stack_bottom;
    char *stack_top;

public:
    // Constructors and Destructor
    Fiber();
    Fiber(void (*function)());
    ~Fiber();

    // Member functions
    const Context &get_context() const;
    void set_context(const Context &newContext);
    void swap_context(Fiber &other);
    void start_execution(Fiber &other);
};

#endif
