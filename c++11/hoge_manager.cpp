#include <stdio.h>
#include <vector>
#include <list>
#include <memory>
#include <cassert>

class Hoge
{
public:
    Hoge() {}
    ~Hoge() {}

private:
    int array[8];
};

class HogeManager
{
public:
    static const int NUM_HOGE = 4;

    HogeManager()
        : m_hoge()
//        , m_free()
//        , m_used()
        , m_latest()
        {
            for (int i = 0; i < NUM_HOGE; i++)
            {
                m_hoge[i] = std::make_shared<Hoge>();
            }
        }

    ~HogeManager()
        {
        }

    std::shared_ptr<Hoge> getLatest()
        {
            return m_latest;
        }

    std::shared_ptr<Hoge> getFree()
        {
            for (int i = 0; i < NUM_HOGE; i++)
            {
                if (m_hoge[i].use_count() == 1)
                {
                    return m_hoge[i];
                }
            }
            
            return nullptr;
        }

    int setLatest(std::shared_ptr<Hoge>& ptr)
        {
            m_latest = ptr;
            return 0;
        }
        
    void printUseCount()
        {
            for (int i = 0; i < NUM_HOGE; i++)
            {
                printf("[%d] %ld ", i, m_hoge[i].use_count());
            }
            printf("\n");
        }

private:
    std::vector<std::shared_ptr<Hoge> > m_hoge;
    std::shared_ptr<Hoge> m_latest;
};

main()
{
    HogeManager hogeManager;

    {
        std::shared_ptr<Hoge> ptr = hogeManager.getFree();
        hogeManager.printUseCount();
        // hogeManager.setLatest(ptr);
    }
}
