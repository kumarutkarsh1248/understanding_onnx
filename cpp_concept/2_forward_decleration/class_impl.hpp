// class_impl.hpp
#ifndef CLASS_IMPL_HPP
#define CLASS_IMPL_HPP

#include<iostream>
using namespace std;
#include "class.hpp"

template <typename T, typename F>
Employee<T, F>::Employee(T age, F salary) : age(age), salary(salary) {}

template <typename T, typename F>
T Employee<T, F>::give_age() { return age; }

template <typename T, typename F>
F Employee<T, F>::give_salary() { return salary; }


//---------------------------------------------------------
write::write()
{
    cout<<"hello its write class"<<endl;
}

#endif
