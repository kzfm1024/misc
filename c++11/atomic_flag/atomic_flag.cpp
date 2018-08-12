#include <iostream>
#include <atomic>
#include <thread>
#include <cstdint>

struct journal_stack {
    std::atomic_flag state;
    int index[8];
    int top;
};

journal_stack* g_journal_stack;
int64_t g_journal[8];

bool push(int i)
{
    std::atomic_flag* sp = &(g_journal_stack->state);
    while (sp->test_and_set(std::memory_order_acquire)) {}
    {
        g_journal_stack->top++;
        g_journal_stack->index[g_journal_stack->top] = i;
    }
    sp->clear(std::memory_order_release);

    return true;
}

bool pop(int& i)
{
    std::atomic_flag* sp = &(g_journal_stack->state);
    while (sp->test_and_set(std::memory_order_acquire)) {}
    {
        i = g_journal_stack->index[g_journal_stack->top];
        g_journal_stack->top--;
    }
    sp->clear(std::memory_order_release);

    return true;
}

void increment()
{
    int index;
    pop(index);
    g_journal[index]++;
    push(index);
}

void worker()
{
    for (int i = 0; i < 1000000; ++i) {        
        increment();
    }
}

int main()
{
    g_journal_stack = (journal_stack*)malloc(sizeof(journal_stack));
    void* p = &(g_journal_stack->state);
    std::atomic_flag* sp = new(p) std::atomic_flag(ATOMIC_FLAG_INIT);
    g_journal_stack->top = -1;
    
    for (int i = 0; i < 8; i++) {
        g_journal[i] = 0;
        push(i);
    }
        
    std::thread t1(worker);
    std::thread t2(worker);
    std::thread t3(worker);
    std::thread t4(worker);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    int sum = 0;
    for (int i = 0; i < 8; i++) {
        sum += g_journal[i];
        std::cout << g_journal[i] << std::endl;
    }
    std::cout << sum << std::endl;

    free(g_journal_stack);
}
