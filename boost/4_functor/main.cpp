// watch mike shah function video and that boost static variatn 
#include<iostream>
#include<vector>
using namespace std;

//functor means function object
//we want to use this object as a function
struct value{
    int m_result1{0};
    double m_result2{0};

    int operator()(int newResult)
    {
        m_result1 = newResult;
        return newResult;
    }

    double operator()(double newResult2)
    {
        m_result2 = newResult2;
        return newResult2;
    }
};

//by default c++ consider deciman value as doble type
int main()
{
    value v;
    v(42); //we are calling the object like a function
    v(4.2);
    cout<<"m_resukt1 "<<v.m_result1<<" m_result2 "<<v.m_result2<<endl;

}