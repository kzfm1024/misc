#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cassert>

//#include <sys/select.h>

static std::mutex s_mutex;

void select_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        fd_set rfds;
        struct timeval tv;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        /* Wait up to five seconds. */
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        
        retval = select(1, &rfds, NULL, NULL, &tv);
        /* Don't rely on the value of tv now! */
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void pselect_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        fd_set rfds;
        struct timespec ts;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);

        /* Wait up to five seconds. */
        ts.tv_sec = 5;
        ts.tv_nsec = 0;
        
        retval = pselect(1, &rfds, NULL, NULL, &ts, NULL);
        /* Don't rely on the value of tv now! */
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

int main()
{
    std::thread t1(select_test);
    std::thread t2(select_test);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        //
        // set the clock forward 1 minute
        //
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += 60;
        int ret = clock_settime(CLOCK_REALTIME, &now);
        assert(ret == 0); // root privilege is necessary for clock_settime()
    }
    t1.join();
    t2.join();
}
