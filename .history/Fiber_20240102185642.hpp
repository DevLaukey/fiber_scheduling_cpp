#ifndef FIBER_HPP
#define FIBER_HPP

#include "context.hpp"

class Fiber
{
public:
    Fiber(void (*func)(), int *data = nullptr);

    void execute();
    Context getContext();
    int *get_data();

private:
    Context context;
    void (*function)();
    int *data_pointer;
};

#endif // FIBER_HPP
