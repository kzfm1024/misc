#include <iostream>
#include <thread>
#include "BlockingQueue.h"

int main()
{
    BlockingQueue<int> que(8);
    
    que.put(1);
    que.put(2);
    que.put(3);
    que.put(4);
    que.put(5);
    que.clear();
    que.put(6);
    que.put(7);
    que.put(8);

    while (!que.empty())
    {
        std::cout << que.take() << " ";
    }
    std::cout << std::endl;
}
