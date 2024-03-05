#include <iostream>
using namespace std;

// Base class
class Shape {
//protected means those attribute and methods will be available in the 
//derived class
protected:
    int width;
    int height;
public:
    Shape(int w, int h) : width(w), height(h) {}
    virtual int area() const = 0; // pure virtual function
};
d
// Derived class
// public means everything public in the parent class will 
//be public in the derived class too
class Rectangle : public Shape {
public:
    Rectangle(int w, int h) : Shape(w, h) {}
    int area() const override {
        return width * height;
    }
};

// Derived class
class Triangle : public Shape {
public:
    Triangle(int w, int h) : Shape(w, h) {}
    int area() const override {
        return width * height / 2;
    }
};

int main() {
    Rectangle rect(5, 4);
    Triangle tri(5, 4);

    cout << "Area of Rectangle: " << rect.area() << endl;
    cout << "Area of Triangle: " << tri.area() << endl;

    return 0;
}
 