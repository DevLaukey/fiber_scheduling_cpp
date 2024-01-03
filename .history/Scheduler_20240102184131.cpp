#include <iostream>
#include <deque>
#include "context.hpp"
#include "Fiber.hpp"

class Scheduler
{
public:
    Scheduler() : context_(new Context()) {}
    ~Scheduler() { delete context_; }

    void spawn(Fiber *f)
    {
        fibers_.push_back(f);
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

private:
    std::deque<Fiber *> fibers_;
    Context *context_;
};

// Fiber class definition
class Fiber
{
public:
    Fiber(void (*func)(), Scheduler *scheduler) : function(func), scheduler_(scheduler) {}

    void execute()
    {
        get_context(&context);
        function();
        scheduler_->fiber_exit(); // Call fiber_exit when the fiber completes
    }

    Context getContext()
    {
        return context;
    }

private:
    Context context;
    void (*function)();
    Scheduler *scheduler_;
};

// Example usage:
void func1()
{
    std::cout << "fiber 1" << std::endl;
}

void func2()
{
    std::cout << "fiber 2" << std::endl;
}

int main()
{
    Scheduler s;

    // Creating fibers
    Fiber f1(&func1, &s);
    Fiber f2(&func2, &s);

    // Spawning fibers
    s.spawn(&f1);
    s.spawn(&f2);

    // Running the scheduler
    s.do_it();

    return 0;
}
