// Fiber.hpp

#pragma once

#include "context.hpp"

class Fiber
{
public:
    Fiber(void (*func)(), int *dataPtr = nullptr); // Updated constructor

    void execute();
    Context getContext();
    int *getDataPtr(); // Getter for data pointer

private:
    Context context;
    void (*function)();
    int *dataPtr; // Shared data pointer
};

int *get_data(); // Declaration for global/shared data
