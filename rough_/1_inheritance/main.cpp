#include <iostream>
using namespace std;

//which ever class signs the contract of using this abstract_employee
//must have to give the implementaion of virtual funciton

//kind of customer which wants implementation of something from the company
class Abstract_Employee
{
// protected:
    virtual void ask_for_promotion() = 0;
};

//behaving as a company
class Employee : public Abstract_Employee
{
// now all the attributes will be available in the derived class also 
//due to this protected 
protected:
    string name;
    string company;
    int age;

public:
    Employee(string name, string company, int age):name(name), company(company),age(age){}
    // these all get set methods are called encapsulation
    void set_name(string name)
    {
        this->name = name;
    }
    string get_name()
    {
        return name;
    }
    void set_company(string company)
    {
        this->company = company;
    }
    string get_company()
    {
        return company;
    }
    void set_age(int age)
    {
        this->age = age;
    }
    int get_age()
    {
        return age;
    }

    void ask_for_promotion()
    {
        if (age > 30)
        {
            cout<<"got promotion"<< endl;
        }else{
             cout<<"no promotion for you"<<endl;
        }
    }
};


//inheritance is privated by default
class Developer : public Employee
{
public:
    string fav_prog_language;

    Developer(string name, string company, int age, string fav_prog_language)
    : Employee(name, company, age)
    {
        this->fav_prog_language = fav_prog_language;
    }
    void fix_bug()
    {
        cout<<get_name()<<" is fixing the bug"<<endl;
    }
};

class Teacher : public Employee
{
public:
    string subject;
    void prepare_lesson()
    {
        cout<<name <<" is preparing "<<subject<<" lesson"<<endl;
    }

    Teacher(string name, string company, int age, string subject)
    : Employee(name, company, age)
    {}
};



int main()
{
    Employee e("rahul", "lora", 80);
    Developer d("utkarsh", "gold", 21, "python");
    Teacher t("ronit", "company", 23, "english");

    Abstract_Employee* a1 = &e;
    Abstract_Employee* a1 = &d;

    // Employee* e1 = a1;  will not work
    Employee e1 = d;
    Developer* d1 = &t;


    return 0;
}