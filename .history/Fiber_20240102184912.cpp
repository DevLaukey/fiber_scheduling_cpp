#include <iostream>
#include <cstdint>
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

private:
    Context context;
    void (*function)();
    int *data_pointer;
};
