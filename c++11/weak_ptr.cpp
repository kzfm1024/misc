#include <iostream>
#include <string>
#include <memory>
#include <cassert>

using namespace std;

class Hoge 
{
    private:
        string s_; 
    public:
        explicit Hoge(const string &s) 
            :s_(s)
        {   
            cout << "Hoge::Hoge(" + s_ + ")" << endl;
        }   

        ~Hoge()
        {   
            cout << "Hoge::~Hoge(" + s_ + ")" << endl;
        }   
};

int main()
{
    weak_ptr<Hoge> weak;

    {   
        shared_ptr<Hoge> shared0(new Hoge("A"));
        assert(shared0.use_count() == 1); 

        shared_ptr<Hoge> shared1= shared0;
        assert(shared0.use_count() == 2); 

        weak = shared0;
        assert(shared0.use_count() == 2); 
    }

    assert(weak.expired());
    return 0;
}
