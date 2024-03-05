#include <iostream>
#include "mlpack.hpp"

using namespace std;

template <typename T1>
class first
{
    public:
    T1 a;
    T1 b;

    first(T1 a, T1 b): a(a), b(b){}
};

template <typename T1, typename T2>
class second : public first<T2>
{
    public:
    T2 c;
    T2 d;

    second(T1 a, T1 b, T2 c, T2 d): first<T1>(a, b), c(c), d(d){}
};

int main()
{
    mlpack::LogSoftMax logSoftMaxLayer;
    //  = new LogSoftMax();
    // layer = &logSoftMaxLayer; 

    cout<<"the code is working fine"<<endl;
    return 0;
}