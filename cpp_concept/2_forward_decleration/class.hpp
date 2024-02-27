// class.hpp
#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>
#include "forward_class.hpp"



template <typename T, typename F>
class Employee
{
public:
    T age;
    F salary;

    /**
     * forward decleration tell that the defination will
     * be available later but it do not give compiler 
     * any information about the size of the forwardly 
     * defined class, so we cant directly make a object
     * out of a class which is defined forward, so we 
     * are using pointer here
    */
    write* w;
    
    Employee(T age, F salary);

    T give_age();

    F give_salary();
};

class write
{
public:
    write();
};

#include "class_impl.hpp"

#endif
