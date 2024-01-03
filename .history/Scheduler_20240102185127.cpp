#include <iostream>
#include <deque>
#include "context.hpp"
#include "Fiber.hpp"

class Scheduler
{
public:
    Scheduler() : context_(nullptr), data_pointer_(nullptr) {}

    void spawn(Fiber *f, int *data = nullptr)
    {
        fibers_.push_back(f);
        data_pointer_ = data;
    }

    void do_it()
    {
        while (!fibers_.empty())
        {
            get_context(context_);

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

    int *get_data()
    {
        return data_pointer_;
    }

private:
    std::deque<Fiber *> fibers_;
    Context *context_;
    int *data_pointer_;
};

void foo(); // Forward declaration of the 'foo' function

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
