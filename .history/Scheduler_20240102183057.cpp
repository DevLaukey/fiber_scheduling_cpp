#include <iostream>
#include <deque>
#include "context.hpp"
#include "Fiber.cpp"

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

// Example usage:
int main()
{
    Scheduler scheduler;

    // Creating fibers
    Fiber fooFiber(&foo);

    // Spawning fibers
    scheduler.spawn(&fooFiber);

    // Running the scheduler
    scheduler.do_it();

    return 0;
}
