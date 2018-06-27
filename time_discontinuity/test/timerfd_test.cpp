#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cassert>

#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>        /* Definition of uint64_t */

#define TIMEOUT 5 /* seconds */

static std::mutex s_mutex;

void timerfd_test_REALTIME_ABSTIME()
{
    auto start = std::chrono::steady_clock::now();
    {
        int fd = timerfd_create(CLOCK_REALTIME, 0);

        struct timespec now;
        struct itimerspec new_value;
        clock_gettime(CLOCK_REALTIME, &now);
        new_value.it_value.tv_sec  = now.tv_sec + TIMEOUT;
        new_value.it_value.tv_nsec = now.tv_nsec;
        
        timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL);

        uint64_t exp;
        ssize_t s = read(fd, &exp, sizeof(uint64_t));
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void timerfd_test_MONOTONIC_ABSTIME()
{
    auto start = std::chrono::steady_clock::now();
    {
        int fd = timerfd_create(CLOCK_MONOTONIC, 0);

        struct timespec now;
        struct itimerspec new_value;
        clock_gettime(CLOCK_MONOTONIC, &now);
        new_value.it_value.tv_sec  = now.tv_sec + TIMEOUT;
        new_value.it_value.tv_nsec = now.tv_nsec;
        
        timerfd_settime(fd, TFD_TIMER_ABSTIME, &new_value, NULL);

        uint64_t exp;
        ssize_t s = read(fd, &exp, sizeof(uint64_t));
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void timerfd_test_REALTIME()
{
    auto start = std::chrono::steady_clock::now();
    {
        int fd = timerfd_create(CLOCK_REALTIME, 0);

        struct itimerspec new_value;
        new_value.it_value.tv_sec  = TIMEOUT;
        new_value.it_value.tv_nsec = 0;
        
        timerfd_settime(fd, 0, &new_value, NULL);

        uint64_t exp;
        ssize_t s = read(fd, &exp, sizeof(uint64_t));
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void timerfd_test_MONOTONIC()
{
    auto start = std::chrono::steady_clock::now();
    {
        int fd = timerfd_create(CLOCK_MONOTONIC, 0);

        struct itimerspec new_value;
        new_value.it_value.tv_sec  = TIMEOUT;
        new_value.it_value.tv_nsec = 0;
        
        timerfd_settime(fd, 0, &new_value, NULL);

        uint64_t exp;
        ssize_t s = read(fd, &exp, sizeof(uint64_t));
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
    std::thread t1(timerfd_test_REALTIME_ABSTIME);
    std::thread t2(timerfd_test_MONOTONIC_ABSTIME);
    std::thread t3(timerfd_test_REALTIME);
    std::thread t4(timerfd_test_MONOTONIC);

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
    t3.join();
    t4.join();
}
