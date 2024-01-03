#pragma once

#include "context.hpp"

class Fiber
{
public:
    Fiber(void (*func)(), int *data = nullptr) : function(func), data_pointer(data) {}

    void execute()
    {
        get_context(&context);
        function();
    }

    Context getContext()
    {
        return context;
    }

    int *get_data()
    {
        return data_pointer;
    }

private:
    Context context;
    void (*function)();
    int *data_pointer;
};
