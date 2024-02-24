#include<iostream>
#include<vector>
using namespace std;

class employee
{
    public:
    employee()
    {
        cout<<"employee is created"<<endl;
    }

    ~employee()
    {
        cout<<"employee is destructed"<<endl;
    }
};


int main()
{
    employee e;
}