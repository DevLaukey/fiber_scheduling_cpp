#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>
#include <deque>
#include "context.hpp"
#include "Fiber.hpp"

class Scheduler
{
public:
    Scheduler() : context_(nullptr) {}

    void spawn(Fiber *f);
    void do_it();
    void fiber_exit();

private:
    std::deque<Fiber *> fibers_;
    Context *context_;
};

#endif // SCHEDULER_HPP
