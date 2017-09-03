#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

//------------------------------------------------------------------------
//                             BlockableQue
//------------------------------------------------------------------------

template<typename T>
class BlockableQue
{
private:
    std::queue<T> que;
    int maxque = 1;
    std::mutex mutexque;
    std::condition_variable pushwait;
    std::condition_variable popwait;

public:
    BlockableQue(int size) { maxque = size; }
    void push(T data);
    T pop();
};

template<typename T>
void BlockableQue<T>::push(T data)
{
    std::unique_lock<std::mutex> lock(mutexque);
    pushwait.wait(lock,
                  [this]{ return (que.size() != static_cast<size_t>(maxque)); });
    que.push(data);
    popwait.notify_one();
}

template<typename T>
T BlockableQue<T>::pop()
{
    std::unique_lock<std::mutex> lock(mutexque);
    popwait.wait(lock,
                 [this]{ return (que.size() != 0); });
    T data = que.front();
    que.pop();
    pushwait.notify_one();
    return data;
}

//------------------------------------------------------------------------
//                             ThreadPool
//------------------------------------------------------------------------

class ThreadPool
{
private:
    BlockableQue<std::function<void()>> jobs;
    std::vector<std::thread*> threads;
    void worker();

    std::mutex m_mutex;
    std::condition_variable m_cond;
    int m_numRunningThreads;

public:
    ThreadPool(int threadcount, int quesize);
    virtual void enque(std::function<void()>);
    virtual void wait();
    void sync();
    int getIndex(std::thread::id id);
};
