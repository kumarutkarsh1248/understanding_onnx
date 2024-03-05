
#define MLPACK_PRINT_DEBUG
#define MLPACK_PRINT_INFO
#define MLPACK_PRINT_WARN


#include "onnx_pb.h"
#include <fstream>
#include <iostream>
#include <memory>
#include<map>
#include <string>
#include "mlpack.hpp"
#include <armadillo>



// #include "model_parser.hpp"

using namespace onnx;
using namespace mlpack;
using namespace ann;
using namespace std;




map<string, double> storedParams;




void printMap(std::map<std::string, double> params)
{
  std::map<std::string, double>::iterator itr;
  for (itr = params.begin(); itr != params.end(); ++itr)
  {
    Log::Info << itr->first << " : " << itr->second << "\n";
  }
}



/**
 * Get the input size of each layer and the output size of the last layer in
 * a vector
 *
 * @param weights The data structure containing the weights and biases
 * of the ONNX model
 * @return The number of nodes in each layer
 */
std::vector<int> findWeightDims
// const ::google::protobuf::RepeatedPtrField => when multiple repeated message is to be hold
(const ::google::protobuf::RepeatedPtrField< ::onnx::TensorProto>& weights)
{

  std::vector<int> dims;
  auto itr = std::begin(weights);

  //*itr => first tenserproto
  //dim is repeated type
  if ((*itr).dims().size() > 2)
  {
    dims.push_back((*itr).dims(1));
    dims.push_back((*itr).dims(0));
    itr += 2; //may be after weight we would have bias term so +=2
  }

  
  for (; itr != std::end(weights); itr += 2)
  {
    if ((*itr).dims().size() == 0)
    {
      itr--;
    }
    else
    {
      dims.push_back((*itr).dims(0));
    }
  }
  itr -= 2;
  dims.push_back((*itr).dims(1));
  return dims;
}








/**
 * Get the mlpack equivalent model of a given ONNX model without
 * the transfer of weights
 *
 * @param graph The ONNX graph containing all the layer details
 * @return An mlpack FFN model corresponding to the ONNX model passed
 */
FFN<> generateModel(GraphProto& graph)
{
  FFN<> mod;
  std::vector<int> dims = findWeightDims(graph.initializer());
  std::vector<int>::iterator itr = dims.begin();
  for (auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr)
  {
    string nodeType = nodeItr -> op_type();
    map<string, double> dimParams;
    std::vector<string> skipLayers = {"Add", "Identity", "Reshape", "Transpose", "Unsqueeze",
    "Shape", "Cast", "Slice", "Concat", "ReduceProd"};
    std::vector<string> dimensionalLayers = {"ConstantOfShape", "Conv", "Gemm", "Matmul",
    "ConvTranspose", "Transformed_linear"};// may be an error with the ConstantOfShape layer
    // layers for merging, currently just one is there
    std::map<std::vector<string>, string> mergeLayers = {
        {{"MatMul", "Add"}, "Transformed_linear"}
        };
    std::map<std::vector<string>, string>::iterator mergeItr;
    for (mergeItr = mergeLayers.begin(); mergeItr != mergeLayers.end(); ++mergeItr)
    {
      std::vector<string> mergeVector = mergeItr -> first;
      int i;
      for (i = 0; i < mergeVector.size(); ++i)
      {
        //cout << "i= " << i << " mervector[i]= " << mergeVector[i] << " op_type= " << (nodeItr + i) -> op_type() << "\n";
        if ((nodeItr + i) == std::end(graph.node()) || mergeVector[i] != (nodeItr + i) -> op_type())
          break;
      }
      //cout << "i= " << i << " mergeVector.size()= " << mergeVector.size() << "\n";
      if (i == mergeVector.size())
      {
        nodeItr += i;
        nodeType = mergeItr -> second;
      }
    }

    if (std::find(skipLayers.begin(), skipLayers.end(), nodeType) != skipLayers.end())
      continue;

    if (std::find(dimensionalLayers.begin(), dimensionalLayers.end(), nodeType) != dimensionalLayers.end())
    {
      dimParams["insize"] = *itr;
      itr++;
      dimParams["outsize"] = *itr;
    }
    if(nodeType == "Conv")
    {
      dimParams["inputwidth"] = storedParams["inputwidth"];
      dimParams["inputheight"] = storedParams["inputheight"];
    }
    std::cout << "Node type: " << nodeType << "\n";
    printMap(dimParams);
    mod.Add(getLayer(*nodeItr, nodeType, dimParams));
  }
  return mod;
}





int main()
{
  return 0;
}