#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cassert>

#include <sys/epoll.h>

static std::mutex s_mutex;
static const int MAX_EVENTS = 5;

void epoll_wait_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct epoll_event events[MAX_EVENTS];
        struct epoll_event ev;
        int timeout;
        int retval;

        int epollfd = epoll_create1(0);

        /* Watch stdin (fd 0) to see when it has input. */
        ev.events = EPOLLIN;
        ev.data.fd = 0;

        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev) == -1)
        {
            perror("epoll_ctl");
            return;
        }

        /* Wait up to five seconds. */
        timeout = 5000;
        
        retval = epoll_wait(epollfd, events, MAX_EVENTS, timeout);
        /* Don't rely on the value of tv now! */
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void epoll_pwait_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct epoll_event events[MAX_EVENTS];
        struct epoll_event ev;
        int timeout;
        int retval;

        int epollfd = epoll_create1(0);

        /* Watch stdin (fd 0) to see when it has input. */
        ev.events = EPOLLIN;
        ev.data.fd = 0;

        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &ev) == -1)
        {
            perror("epoll_ctl");
            return;
        }

        /* Wait up to five seconds. */
        timeout = 5000;
        
        retval = epoll_pwait(epollfd, events, MAX_EVENTS, timeout, NULL);
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
    std::thread t1(epoll_wait_test);
    std::thread t2(epoll_pwait_test);
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
