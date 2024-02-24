#include <iostream>
#include <boost/variant.hpp>

// Define a visitor class that defines overloaded operator() for each type
class MyVisitor : public boost::static_visitor<void> {
public:
    void operator()(int value) const {
        std::cout << "Integer value: " << value << std::endl;
    }

    void operator()(double value) const {
        std::cout << "Double value: " << value << std::endl;
    }

    void operator()(const std::string& value) const {
        std::cout << "String value: " << value << std::endl;
    }
};

int main() {
    // Define a boost::variant with int, double, and std::string
    boost::variant<int, double, std::string> myVariant;
    MyVisitor v;

    // Set the variant to different types
    myVariant = 42;


    // v(myVariant);  ===> we cant directly pass the static_variant into functor

    /**
     * Although we don't explicitly create an object of the MyVisitor class, 
     * an anonymous instance of the MyVisitor class is created inside the 
     * boost::apply_visitor function call:
    */

    myVariant = 3.14;
    boost::apply_visitor(MyVisitor(), myVariant);

    myVariant = "Hello";
    boost::apply_visitor(MyVisitor(), myVariant);

    return 0;
}
