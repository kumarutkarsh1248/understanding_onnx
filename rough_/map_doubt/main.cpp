#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "mlpack.hpp"

using namespace mlpack;
using namespace std;


int main()
{
  FFN<> ffn;
  ffn.InputDimensions() = std::vector<size_t>({1});
  // ffn.Reset();

  NoRegularizer regularizer;
  Layer<> *linear_no_bias = new LinearNoBias(100, regularizer);
  ffn.Add(linear_no_bias);
  ffn.Reset();


  Layer<> *add = new Add();
  ffn.Add(add);
  ffn.Reset();

  Layer<> *relu = new ReLU();
  ffn.Add(relu);


  ffn.Reset();


  


  

  return 0;
}