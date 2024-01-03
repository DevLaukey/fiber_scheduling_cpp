#include <deque>
#include "Fiber.hpp" // Assuming you have the Fiber class defined

class Scheduler
{
private:
    std::deque<Fiber *> fibers_;
    Context *context_;

public:
    // Constructor
    Scheduler()
    {
        context_ = new Context();
    }

    // Destructor
    ~Scheduler()
    {
        delete context_;
    }

    // Method to spawn a fiber
    void spawn(Fiber *f)
    {
        fibers_.push_back(f);
    }

    // Method to execute fibers in a round-robin fashion
    void do_it()
    {
        // Return here to re-enter the scheduler
        if (fibers_.empty())
        {
            return;
        }

        // Get the context of the scheduler
        get_context(context_);

        while (!fibers_.empty())
        {
            // Transfer control to the next fiber
            Fiber *currentFiber = fibers_.front();
            fibers_.pop_front();

            // Jump back to the scheduler 'loop'
            set_context(context_);

            // Jump to the task
            const Context &fiberContext = currentFiber->get_context();
            set_context(fiberContext);
        }
    }

    // Method to exit the current fiber and return to the scheduler
    void fiber_exit()
    {
        // Jump back to the scheduler 'loop'
        set_context(context_);
    }
};
