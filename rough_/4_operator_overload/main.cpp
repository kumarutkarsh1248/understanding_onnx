#include <iostream>
#include <vector>
#include <iterator>
#include <map>
#include <string>
using namespace std;

template <typename T>
std::ostream &
operator<<(std::ostream &out, const std::vector<std::vector<T>> &v)
{
    if (!v.empty())
    {
        out << '[';
        for (int i = 0; i < v.size(); i++)
            std::copy(
                v.at(i).begin(), v.at(i).end(), std::ostream_iterator<T>(out, ","));
        out << "\b]";
    }
    return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v)
{
    if (!v.empty())
    {
        out << '[';
        std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
        out << "\b\b]";
    }
    return out;
}

template <typename T1, typename T2>
std::ostream &
operator<<(std::ostream &out, const std::map<T1, T2> &m)
{
    out << "{\n";
    for (auto it : m)
    {
        out << "  {" << it.first << " : " << it.second << "}\n";
    }
    out << "\b}";
    return out;
}

int main()
{
    // Create a vector of vectors
    std::vector<std::vector<int>> matrix1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    std::vector<int> matrix2 = {1, 2, 3};
    map<string, string> m = {
        {"hellow", "world"},
        {"this", "is"}};

    // Output the matrix using the overloaded operator<< function
    std::cout << "Matrix: " << matrix1 << endl
              << matrix2 << endl
              << m << std::endl;

    return 0;
}