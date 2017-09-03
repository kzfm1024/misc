#include <unistd.h>
#include "ThreadPool.h"

void ThreadPool::worker()
{
    for (;;)
    {
        auto job = jobs.pop();
        if (job == nullptr)
            return;
        job();
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            --m_numRunningThreads;
            m_cond.notify_one();
        }
    }
}

ThreadPool::ThreadPool(int threadcount, int quesize)
    : jobs(quesize)
    , m_mutex()
    , m_cond()
    , m_numRunningThreads(0)
{
    for (int i = 0; i < threadcount; i++)
    {
        std::thread *t = new std::thread([this]{worker();});
        threads.push_back(t);
    }
}

void ThreadPool::enque(std::function<void()> job)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    ++m_numRunningThreads;
    jobs.push(job);
}

void ThreadPool::sync()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond.wait(lock, [this]{
            return this->m_numRunningThreads == 0;
        });
}

void ThreadPool::wait()
{
    // stop all worker threads by nullptr as job argument
    for (unsigned int i = 0; i < threads.size(); i++)
        jobs.push(nullptr);

    // join all threads
    for (auto t : threads)
    {
        t->join();
        delete t;
    }

    threads.clear();
}

int ThreadPool::getIndex(std::thread::id id)
{
    for (int i = 0; i < static_cast<int>(threads.size()); i++)
    {
        auto t = threads[i];
        if (t->get_id() == id) return i;
    }

    return -1;
}
