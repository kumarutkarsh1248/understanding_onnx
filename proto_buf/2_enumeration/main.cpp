#include<iostream>
#include "person.pb.h"
using namespace std;


int main()
{
    PERSON::Person person;

    //setting name, id, email field int he person message
    person.set_name("name");
    person.set_id(22);
    person.set_email("email");

    cout<<"this is the phone type "<<person.phone_type()<<endl;
    person.set_phone_type(PERSON::Person::MOBILE);
    cout<<person.DebugString()<<endl;

}