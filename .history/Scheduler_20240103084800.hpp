#pragma once

#include <deque>
#include "Fiber.hpp"

class Scheduler
{
private:
    std::deque<Fiber *> fibers_;
    Context context_;

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

            const Context &fiberContext = currentFiber->get_context();
            set_context(&fiberContext);
        }
    }
};
