// main.cpp
#include "class.hpp"
#include <iostream>

int main()
{
    int age = 20;
    float salary = 22.2;
    Employee<int, float> e(age, salary);

    std::cout << e.give_salary() << " " << e.give_age() << std::endl;

    return 0;
}
