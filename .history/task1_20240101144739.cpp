#include "context.hpp"
#include <iostream>

volatile int x = 0;

void foo()
{
    std::cout << "You called foo" << std::endl;
    set_context(nullptr); // Returning to the main context
}

int main()
{
    char data[4096];
    char *sp = data + sizeof(data);

    Context c;
    c.rsp = sp;
    c.rip = (void *)&foo;

    if (get_context(&c) == 0)
    {
        // This is the first time, execute the fiber's body
        set_context(&c);
    }

    // The following code will be executed after the first switch
    if (x == 0)
    {
        x++;
        set_context(&c);
    }

    return 0;
}
