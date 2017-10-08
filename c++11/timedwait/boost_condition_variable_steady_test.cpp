#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cassert>
#include <boost/thread.hpp>

static boost::mutex s_mutex;
static boost::condition_variable_steady s_cond;

static std::mutex s_mutex_np;
static boost::condition_variable_steady_np s_cond_np;

void timedwait_steady(int id)
{
    boost::unique_lock<boost::mutex> lock(s_mutex);
    
    auto start = boost::chrono::steady_clock::now();
    boost::cv_status ret = s_cond.wait_until(lock, start + boost::chrono::seconds(5));

    auto end = boost::chrono::steady_clock::now();
    boost::chrono::duration<double> duration = end - start;

    if (ret == boost::cv_status::timeout)
    {
        std::cout << "t" << id << " timeout: " << duration.count() << std::endl;
    }
    else
    {
        std::cout << "t" << id << " success: " << duration.count() << std::endl;
    }
}

void timedwait_steady_np(int id)
{
    std::unique_lock<std::mutex> lock(s_mutex_np);
    
    auto start = boost::chrono::steady_clock::now();
    boost::cv_status ret = s_cond_np.wait_until(lock, start + boost::chrono::seconds(5));

    auto end = boost::chrono::steady_clock::now();
    boost::chrono::duration<double> duration = end - start;

    if (ret == boost::cv_status::timeout)
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
    std::thread t1(timedwait_steady, 1);
    std::thread t2(timedwait_steady_np, 2);
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
