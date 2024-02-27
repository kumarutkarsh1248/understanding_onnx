#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include<map>
#include<vector>
using namespace std;

using vartype = boost::variant<int, float, double, string>;



int main()
{
    map<string, string> mapped_names;
    mapped_names["int"] = "Int";
    mapped_names["float"] = "Float";
    mapped_names["double"] = "Double";
    mapped_names["string"] = "String";


    map<string, vector<string>> layer = mapped_names[vartype];
}