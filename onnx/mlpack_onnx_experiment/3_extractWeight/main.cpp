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




/**
 * Transfer the weights of the ONNX model to the mlpack model
 *
 * @param graph The ONNX graph containing all the weights and layer details
 * @param weightMatrix The matrix containing the mlpack model's weights
 */

// initializer{
//     dim: 4;
//     dim: 35;
// }initializer{
//     dim: 35;
//     dim: 8;
// }
// initializer{
//     dim: 8;
//     dim: 9;
// }
void extractWeights(GraphProto& graph, arma::mat& weightMatrix)
{
    // weights => list of tensors
  auto& weights = graph.initializer();
  int totalWeights = 0;

  //list of size number of tensors in the initializer
  int weightDims[weights.size()];
  int wtNumber = 0;

  for(TensorProto weight:weights)
  {
    //in case of empty initializer
    if (weight.dims().size() == 0)
        continue;

    //dim will be of repeated type so there is method dims_size()
    int weightSize = weight.dims(0);
    // dim_size() => total number of dim value in the tensor
    for (int i = 1; i < weight.dims_size(); ++i)
    {
      weightSize *= weight.dims(i);
    }

    // so finally this weightdim will contails three elements
    // and this will be like [4*35, 35*8, 8*9]
    weightDims[wtNumber++] = weightSize;

    //and this totalweight =  4*35 + 35*8 + 8*9
    totalWeights += weightSize;
  }




  wtNumber = 0;// reinitialize for use
  int count = 0;
  weightMatrix.set_size(totalWeights, 1);
  for(TensorProto weight:weights)
  {
    if (weight.dims().size() == 0)
      continue;


    //my models do not have any raw data so i can skip
    if (weight.has_raw_data())
    {
      std::string rawData = weight.raw_data();
      const char* ws = rawData.c_str();
      for (int i = 0; i < weightDims[wtNumber] * 4 - 4; i += 4)
      {
        float wt;
        char t[] = {ws[i], ws[i+1], ws[i+2], ws[i+3]};
        memcpy(&wt, &t, sizeof(float));
        weightMatrix(count++, 0) = wt;
      }
    }
    else if (weight.data_type() == 1)// for float type
    {
      for (int i = 0; i < weightDims[wtNumber]; ++i)
      {
        weightMatrix(count++, 0) = weight.float_data()[i];
      }
    }
//     wtNumber++;
  }
}







int main()
{
    onnx::ModelProto onnxModel;
    std::ifstream in("onnx_linear_model.onnx", std::ios_base::binary);
    onnxModel.ParseFromIstream(&in);
    in.close();

    onnx::GraphProto graph = onnxModel.graph();


    arma::mat weightMatrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}

    };

    // extractWeights(graph, weightMatrix);


    cout<<"weight extracted successfully"<<endl;
    weightMatrix.print("this is the extracted weight matrix");

    return 0;
}






