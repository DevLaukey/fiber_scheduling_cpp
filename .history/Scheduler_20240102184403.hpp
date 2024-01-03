#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <deque>
#include "context.hpp"
#include "Fiber.hpp"

class Scheduler
{
public:
    Scheduler() : context_(nullptr) {}

    void spawn(Fiber *f)
    {
        fibers_.push_back(f);
    }

    void do_it()
    {
        while (!fibers_.empty())
        {
            get_context(&context_);

            if (!fibers_.empty())
            {
                Fiber *f = fibers_.front();
                fibers_.pop_front();
                set_context(&(f->getContext()));
            }
        }
    }

    void fiber_exit()
    {
        // Transfer control back to the scheduler loop
        set_context(context_);
    }

private:
    std::deque<Fiber *> fibers_;
    Context *context_;
};

#endif // SCHEDULER_HPP
