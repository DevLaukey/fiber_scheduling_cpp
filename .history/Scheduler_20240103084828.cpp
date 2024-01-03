#include <deque>
#include "Fiber.hpp" 

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

  
  
    // Method to exit the current fiber and return to the scheduler
    void fiber_exit()
    {
        // Jump back to the scheduler 'loop'
        set_context(context_);
    }
};
