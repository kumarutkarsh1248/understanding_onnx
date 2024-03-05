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


// #include "model_parser.hpp"

using namespace onnx;
using namespace mlpack;
using namespace ann;
using namespace std;









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








int main()
{
    onnx::ModelProto onnxModel;
    std::ifstream in("onnx_linear_model.onnx", std::ios_base::binary);
    onnxModel.ParseFromIstream(&in);
    in.close();

    onnx::GraphProto graph = onnxModel.graph();

    // int i = 0;
    // for (const onnx::NodeProto& node : graph.node()) {
    //     if(node.output(0) == "Constant312")
    //     {
    //         if(node.attribute(0).name() == "value")
    //         {
    //             cout<<"mil gaya"<<endl;
    //             break;
    //         }
            
    //     }
    // }

    ::google::protobuf::RepeatedPtrField< ::onnx::TensorProto> weights = graph.initializer();
    cout<<"the size of the weights is "<<weights.size()<<endl;

    std::vector<int> v = findWeightDims(weights);

    cout<<"the size of the v is  "<<v.size()<<endl;
    cout<<"and the vector is "<<endl;
    for(int i:v)
    {
      cout<<i<<endl;
    }



    return 0;


}










