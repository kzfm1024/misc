#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cassert>

#include <signal.h>
#include <string.h>

#define TIMEOUT 5 /* seconds */

static std::mutex s_mutex;
static std::condition_variable s_cond;

static void timer_handler(int signum)
{
    s_cond.notify_one();    
}

void timer_test_REALTIME_ABSTIME()
{
    auto start = std::chrono::steady_clock::now();
    {
        int ret;
        std::unique_lock<std::mutex> lock(s_mutex);

        struct sigaction act, oldact;
        memset(&act, 0, sizeof(struct sigaction));
        memset(&oldact, 0, sizeof(struct sigaction));
 
        act.sa_handler = timer_handler;
        act.sa_flags = SA_RESTART;
        ret = sigaction(SIGALRM, &act, &oldact);
        assert(ret == 0);

        timer_t tid;
        struct timespec now;
        struct itimerspec itval;
        clock_gettime(CLOCK_REALTIME, &now);
        itval.it_value.tv_sec = now.tv_sec + TIMEOUT;
        itval.it_value.tv_nsec = now.tv_nsec;
        itval.it_interval.tv_sec = 0;
        itval.it_interval.tv_nsec = 0;
 
        ret = timer_create(CLOCK_REALTIME, NULL, &tid);
        assert(ret == 0);

        ret = timer_settime(tid, TIMER_ABSTIME, &itval, NULL);
        assert(ret == 0);
        
        s_cond.wait(lock);

        timer_delete(tid);
        sigaction(SIGALRM, &oldact, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void timer_test_MONOTONIC_ABSTIME()
{
    auto start = std::chrono::steady_clock::now();
    {
        int ret;        
        std::unique_lock<std::mutex> lock(s_mutex);

        struct sigaction act, oldact;
        memset(&act, 0, sizeof(struct sigaction));
        memset(&oldact, 0, sizeof(struct sigaction));
 
        act.sa_handler = timer_handler;
        act.sa_flags = SA_RESTART;
        ret = sigaction(SIGALRM, &act, &oldact);
        assert(ret == 0);

        timer_t tid;
        struct timespec now;
        struct itimerspec itval;
        clock_gettime(CLOCK_MONOTONIC, &now);
        itval.it_value.tv_sec = now.tv_sec + TIMEOUT;
        itval.it_value.tv_nsec = now.tv_nsec;
        itval.it_interval.tv_sec = 0;
        itval.it_interval.tv_nsec = 0;
 
        ret = timer_create(CLOCK_MONOTONIC, NULL, &tid);
        assert(ret == 0);

        ret = timer_settime(tid, TIMER_ABSTIME, &itval, NULL);
        assert(ret == 0);
        
        s_cond.wait(lock);

        timer_delete(tid);
        sigaction(SIGALRM, &oldact, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void timer_test_REALTIME()
{
    auto start = std::chrono::steady_clock::now();
    {
        int ret;
        std::unique_lock<std::mutex> lock(s_mutex);
        
        struct sigaction act, oldact;
        memset(&act, 0, sizeof(struct sigaction));
        memset(&oldact, 0, sizeof(struct sigaction));

        act.sa_handler = timer_handler;
        act.sa_flags = SA_RESTART;
        ret = sigaction(SIGALRM, &act, &oldact);
        assert(ret == 0);
 
        timer_t tid;
        struct itimerspec itval;
        itval.it_value.tv_sec = TIMEOUT;
        itval.it_value.tv_nsec = 0;
        itval.it_interval.tv_sec = 0;
        itval.it_interval.tv_nsec = 0;
 
        ret = timer_create(CLOCK_REALTIME, NULL, &tid);
        assert(ret == 0);

        ret = timer_settime(tid, 0, &itval, NULL);
        assert(ret == 0);
        
        s_cond.wait(lock);

        timer_delete(tid);
        sigaction(SIGALRM, &oldact, NULL);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::cout << __func__ << ": " << duration.count() << std::endl;
    }
}

void timer_test_MONOTONIC()
{
    auto start = std::chrono::steady_clock::now();
    {
        int ret;
        std::unique_lock<std::mutex> lock(s_mutex);

        struct sigaction act, oldact;
        memset(&act, 0, sizeof(struct sigaction));
        memset(&oldact, 0, sizeof(struct sigaction));
 
        act.sa_handler = timer_handler;
        act.sa_flags = SA_RESTART;
        ret = sigaction(SIGALRM, &act, &oldact);
        assert(ret == 0);
 
        timer_t tid;
        struct itimerspec itval;
        itval.it_value.tv_sec = TIMEOUT;
        itval.it_value.tv_nsec = 0;
        itval.it_interval.tv_sec = 0;
        itval.it_interval.tv_nsec = 0;
 
        ret = timer_create(CLOCK_MONOTONIC, NULL, &tid);
        assert(ret == 0);

        ret = timer_settime(tid, 0, &itval, NULL);
        assert(ret == 0);
        
        s_cond.wait(lock);

        timer_delete(tid);
        sigaction(SIGALRM, &oldact, NULL);
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
    std::thread t1(timer_test_REALTIME_ABSTIME);
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

    std::thread t2(timer_test_MONOTONIC_ABSTIME);
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
    t2.join();

    std::thread t3(timer_test_REALTIME);
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
    t3.join();

    std::thread t4(timer_test_MONOTONIC);
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
    t4.join();
}
