#include <boost/variant.hpp>
#include <iostream>
#include <string>

int main() {
    // Define a variant that can hold an integer or a string
    boost::variant<int, std::string> value;

    // Assign an integer
    value = 42;
    std::cout << "Value: " << value << std::endl;

    // Assign a string
    value = "Hello, Boost!";

    // Retrieve and print the string value
    std::cout << "Value: " << value << std::endl;

    return 0;
}
