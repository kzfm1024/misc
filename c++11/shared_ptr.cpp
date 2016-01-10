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
    shared_ptr<Hoge> hoge_ptr0;

    {
        cout << "---create A and B---" << endl;
        shared_ptr<Hoge> hoge_ptr1(new Hoge("A"));
        shared_ptr<Hoge> hoge_ptr2(new Hoge("B"));
        shared_ptr<Hoge> hoge_ptr3(new Hoge("C"));

        assert(hoge_ptr1.use_count() == 1);
        assert(hoge_ptr2.use_count() == 1);
        assert(hoge_ptr3.use_count() == 1);

        cout << "---delete A---" << endl;
        hoge_ptr0 = hoge_ptr1 = hoge_ptr2;

        
        assert(hoge_ptr2.use_count() == 3);
        assert(hoge_ptr3.use_count() == 1);

        cout << "---delete C---" << endl;
    }

    cout << "---delete B---" << endl;
    return 0;
}
