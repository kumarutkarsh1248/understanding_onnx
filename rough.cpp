#include<iostream>
#include<string>
using namespace std;

int main()
{
    string str;
    getline(cin, str);
    if(str.empty())
    {
        cout<<"string is empty"<<endl;
    }else{
        cout<<"string is not empty"<<endl;
    }

    return 0;

}