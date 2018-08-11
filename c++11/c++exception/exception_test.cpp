#include <iostream>
#include <thread>
#include "backtrace.h"

void on_new_failed(void)
{
    write_backtrace (STDOUT_FILENO);
    throw std::bad_alloc ();
}

void myfunc3(void)
{
    int *p = new int[0xffffffffff];	// allocate too large memory
}

static void myfunc2(void) // "static" means don't export the symbol...
{
    myfunc3 ();
}

void myfunc(int ncalls)
{
    if (ncalls > 1)
        myfunc (ncalls - 1);
    else
        myfunc2 ();
}

void exception_test(void)
{
    try {
        myfunc (3);
    } catch (const std::exception & ex) {
        std::cerr << __func__ << " ex.what() " << ex.what () << std::endl;
        throw;
    }
}

int main(void)
{
    std::new_handler handler = on_new_failed;
    std::set_new_handler (handler);

    try {
        std::thread t (exception_test);
        t.join ();
    } catch (const std::exception & ex) {
        //
        // can't catch exceptions in other threads
        //
        std::cerr << __func__ << " ex.what() " << ex.what () << std::endl;
    }
}
