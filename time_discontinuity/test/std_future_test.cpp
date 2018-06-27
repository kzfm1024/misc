#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>
#include <cassert>

static std::mutex s_mutex;

void future_wait_for_test()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    auto start = std::chrono::steady_clock::now();  
    std::future_status ret = f.wait_for(std::chrono::seconds(5));
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (ret == std::future_status::timeout)
        {
            std::cout << __func__ << " timeout: " << duration.count() << std::endl;
        }
        else
        {
            std::cout << __func__ << " success: " << duration.count() << std::endl;      
        }
    }
}

void future_wait_until_test()
{
    std::promise<int> p;
    std::future<int> f = p.get_future();

    auto start = std::chrono::steady_clock::now();  
    std::future_status ret = f.wait_until(start + std::chrono::seconds(5));
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = end - start;

    {
        std::lock_guard<std::mutex> lock(s_mutex);
        if (ret == std::future_status::timeout)
        {
            std::cout << __func__ << " timeout: " << duration.count() << std::endl;
        }
        else
        {
            std::cout << __func__ << " success: " << duration.count() << std::endl;      
        }
    }
}

int main()
{
    std::thread t1(future_wait_for_test);
    std::thread t2(future_wait_until_test);
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
