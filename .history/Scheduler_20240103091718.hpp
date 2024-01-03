#pragma once

#include <deque>
#include "Fiber.hpp"

class Scheduler
{
private:
    std::deque<Fiber *> fibers_;
    Context context_;
    int *sharedData;

public:
    Scheduler() {}

    ~Scheduler()
    {
        for (auto fiber : fibers_)
        {
            delete fiber;
        }
    }

    void spawn(Fiber *f)
    {
        fibers_.push_back(f);
    }

     void do_it()
    {
        if (fibers_.empty())
        {
            return;
        }

        get_context(&context_);

        while (!fibers_.empty())
        {
            Fiber *currentFiber = fibers_.front();
            fibers_.pop_front();

            set_context(&context_);

            // Create a non-const copy of the context
            Context fiberContext = currentFiber->get_context();

            set_context(&fiberContext);
        }
    }
    void fiber_exit()
    {
        set_context(&context_);
    }
    int *get_shared_data() const
    {
        return sharedData;
    }
};

extern Scheduler s;
