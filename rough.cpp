#define MLPACK_PRINT_DEBUG
#define MLPACK_PRINT_INFO
#define MLPACK_PRINT_WARN

// #define MLPACK_SUPPRESS_FATAL

#include<iostream>
#include<string>
#include "mlpack.hpp"

using namespace mlpack;
using namespace std;



int main()
{
    Log::Info <<"this is the info call"<<endl;
    Log::Fatal <<"this is the fatal call";

    return 0;
}
