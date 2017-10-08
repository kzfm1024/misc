#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <time.h>
#include <assert.h>
 
static std::mutex s_mutex;
static std::condition_variable s_cond;

void timedwait(int id)
{
    std::unique_lock<std::mutex> lock(s_mutex);

    auto start = std::chrono::system_clock::now();
    std::cv_status ret = s_cond.wait_until(lock, start + std::chrono::seconds(5));

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> delta = end - start;

    if (ret == std::cv_status::timeout)
    {
        std::cout << "t" << id << " timeout: " << delta.count() << std::endl;
    }
    else
    {
        std::cout << "t" << id << " success: " << delta.count() << std::endl;
    }
}

void timedwait_monotonic(int id)
{
    std::unique_lock<std::mutex> lock(s_mutex);

    //
    // http://en.cppreference.com/w/cpp/thread/condition_variable/wait_until
    //
    // The clock tied to timeout_time is used, which is not required to be a 
    // monotonic clock.There are no guarantees regarding the behavior of this
    // function if the clock is adjusted discontinuously, but the existing
    // implementations convert timeout_time from Clock to std::chrono::system_clock
    // and delegate to POSIX pthread_cond_timedwait so that the wait honors 
    // ajustments to the system clock, but not to the the user-provided Clock. 
    // In any case, the function also may wait for longer than until after 
    // timeout_time has been reached due to scheduling or resource contention delays.
    //
    auto start = std::chrono::steady_clock::now();
    std::cv_status ret = s_cond.wait_until(lock, start + std::chrono::seconds(5));

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    if (ret == std::cv_status::timeout)
    {
        std::cout << "t" << id << " timeout: " << duration.count() << std::endl;
    }
    else
    {
        std::cout << "t" << id << " success: " << duration.count() << std::endl;
    }
}

int main()
{
    //
    // do notify_one() after 1 seconds
    //
    std::thread t1(timedwait, 1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::unique_lock<std::mutex> lock(s_mutex);
        s_cond.notify_one();
    }
    t1.join();

    //
    // do nothing - timedwait thread will be timed out
    //
    std::thread t2(timedwait, 2);
    {
        ;
    }
    t2.join();

    //
    // set the clock forward 1 minute
    //
    std::thread t3(timedwait, 3);
    std::thread t4(timedwait_monotonic, 4);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        struct timespec now;
        clock_gettime(CLOCK_REALTIME, &now);
        now.tv_sec += 60;
        int ret = clock_settime(CLOCK_REALTIME, &now);
        assert(ret == 0); // root privilege is necessary for clock_settime()
    }
    t3.join();
    t4.join();
}
