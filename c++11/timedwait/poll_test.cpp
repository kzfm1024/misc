#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cassert>

#include <sys/poll.h>

static std::mutex s_mutex;

void poll_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct pollfd fds[1];
        int timeout;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        fds[0].fd = 0;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        /* Wait up to five seconds. */
        timeout = 5000;
        
        retval = poll(fds, 1, timeout);
        /* Don't rely on the value of tv now! */
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void ppoll_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct pollfd fds[1];
        struct timespec ts;
        int retval;

        /* Watch stdin (fd 0) to see when it has input. */
        fds[0].fd = 0;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        /* Wait up to five seconds. */
        ts.tv_sec = 5;
        ts.tv_nsec = 0;
        
        retval = ppoll(fds, 1, &ts, NULL);
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
    std::thread t1(poll_test);
    std::thread t2(ppoll_test);
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
