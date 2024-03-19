#include <iostream>
#include <map>
#include <string>
#include <vector>
// #include <algorithm> // Include algorithm header for std::copy
#include <iterator> // Include iterator header for std::ostream_iterator

using namespace std;

// Overload the << operator for vector printing
template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v)
{
  if (!v.empty())
  {
    out << '[';
    std::copy(v.begin(), std::prev(v.end()), std::ostream_iterator<T>(out, ", "));
    if (!v.empty())
      out << v.back(); // Print the last element without a comma
    out << ']';
  }
  return out;
}

int main()
{
  map<string, vector<int>> v;
  v["hello"] = {1, 2, 3};
  v["world"] = {4, 5, 6};

  vector<int> v1 = v["rahul"];

  for (auto elements : v)
  {
    cout << elements.first << ": " << elements.second << endl;
  }

  return 0;
}