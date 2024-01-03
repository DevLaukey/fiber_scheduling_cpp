// Fiber.hpp

#pragma once

#include "context.hpp"

// Declaration for fiber_exit
void fiber_exit();

class Fiber
{
public:
    Fiber(void (*func)(), int *dataPtr = nullptr);

    void execute();
    Context getContext();
    int *getDataPtr();

private:
    Context context;
    void (*function)();
    int *dataPtr;
};

int *get_data(); // Declaration for global/shared data
