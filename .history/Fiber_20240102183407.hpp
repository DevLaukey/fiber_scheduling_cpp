#pragma once

#include "context.hpp"

class Fiber
{
public:
        Fiber(void (*function)()) : function_(function), context_() {}

        Context &getContext() { return context_; }

        void execute()
        {
            function_();
            // Upon completion, return to the scheduler
            set_context(&context_);
        }

    private:
        void (*function_)();
        Context context_;
};
