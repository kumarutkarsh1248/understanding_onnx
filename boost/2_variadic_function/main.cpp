#include<iostream>
using namespace std;

// base class
template<typename T>
T sum(T arg)
{
    return arg;
}

// parameter pack
template<typename T, typename... Args>
T sum(T first, Args... args)
{
    return first + sum(args...);
}




int main(){
    cout<<sum<double>(1, 2.2)<<endl;

}