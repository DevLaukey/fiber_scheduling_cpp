// fiber_context.hpp
#pragma once

extern "C"
{
#include "context_asm.s" // Include your assembly code here
}

struct Context
{
    void *rip, *rsp;
    void *rbx, *rbp, *r12, *r13, *r14, *r15;
};

extern "C" void get_context(Context *c);
extern "C" void set_context(Context *c);
extern "C" void swap_context(Context *out, Context *in);
