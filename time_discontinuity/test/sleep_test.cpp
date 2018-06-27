#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <cassert>

#include <unistd.h>

static std::mutex s_mutex;

void sleep_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        sleep(5); // 5 seconds
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void usleep_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        usleep(5000000); // 5 seconds
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void nanosleep_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct timespec ts;
        ts.tv_sec = 5;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void clock_nanosleep_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct timespec ts;
        ts.tv_sec = 5;
        ts.tv_nsec = 0;
        clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }

}

void clock_nanosleep_REALTIME_ABSTIME_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 5;
        ts.tv_nsec = 0;
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &ts, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void clock_nanosleep_MONOTONIC_ABSTIME_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        ts.tv_sec += 5;
        ts.tv_nsec = 0;
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void sleep_for_test()
{
    auto start = std::chrono::steady_clock::now();
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));
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
    std::thread t1(sleep_test);
    std::thread t2(usleep_test);
    std::thread t3(nanosleep_test);
    std::thread t4(clock_nanosleep_test);
    std::thread t5(clock_nanosleep_REALTIME_ABSTIME_test);
    std::thread t6(clock_nanosleep_MONOTONIC_ABSTIME_test);
    std::thread t7(sleep_for_test);
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
    t5.join();
    t6.join();
    t7.join();
}
