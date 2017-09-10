#include <iostream>
#include <thread>
#include <atomic>
#include <cassert>
#include "BlockingQueue.h"
#include <sched.h>

std::atomic<int> s_flag;
BlockingQueue<int> s_que(1);

int set_sched_priority(int policy, int prio)
{
    struct sched_param param;
    sched_getparam(0, &param);
    int prio1 = param.sched_priority;
    
    param.sched_priority = prio;
    int ret =  sched_setscheduler(0, policy, &param);
    assert(ret == 0);
    
    sched_getparam(0, &param);
    int prio2 = param.sched_priority;
    std::cout << "set_sched_priority:" << prio1 << "->" << prio2 << std::endl;
    
    return ret;
}

void producer()
{
    set_sched_priority(SCHED_FIFO, 56);
    
    for (int i = 0; i < 10; i++)
    {
        s_que.put(i);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    s_flag = 0;
    s_que.put(-1);

    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main()
{
    set_sched_priority(SCHED_FIFO, 55);
    
    std::thread th(producer);
    s_flag = 1;

    while (s_flag)
    {
        std::cout << s_que.take() << std::endl;
    }

    std::cout << "waiting for producer thread..." << std::endl;
    th.join();
}
