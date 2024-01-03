// Fiber.hpp

#pragma once

#include "context.hpp"

class Fiber
{
public:
    Fiber(void (*func)(), int *dataPtr = nullptr);

    void execute();
    Context getContext();
    int *getDataPtr();
    void fiber_exit();

private:
    Context context;
    void (*function)();
    int *dataPtr;
};

// Implementation of fiber_exit
void Fiber::fiber_exit()
{
    // Perform any necessary cleanup or handling here
    // For simplicity, we'll just return for now
    return;
}
