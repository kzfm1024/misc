#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BlockingQueue
{
public:
    BlockingQueue(size_t size = 1)
        : m_maxsize(size), m_queue()
        , m_mutex(), m_back_cond(), m_front_cond() {}
    ~BlockingQueue() {}

    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }
    
    void clear()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (!m_queue.empty())
        {
            m_queue.pop();
        }
        m_back_cond.notify_one();
    }

    void put(T data)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_back_cond.wait(lock, [this]{ return (m_queue.size() != m_maxsize); });
        m_queue.emplace(data);
        m_front_cond.notify_one();
    }
    
    T take()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_front_cond.wait(lock, [this]{ return (m_queue.size() != 0); });
        T data = m_queue.front();
        m_queue.pop();
        m_back_cond.notify_one();
        return data;
    }

private:
    size_t m_maxsize;
    std::queue<T> m_queue;    
    std::mutex m_mutex;
    std::condition_variable m_back_cond;
    std::condition_variable m_front_cond;
};
