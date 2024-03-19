#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace onnx;



/**
 * Transfer the weights of the ONNX model to the mlpack model
 *
 * @param graph The ONNX graph containing all the weights and layer details
 * @param weightMatrix The matrix containing the mlpack model's weights
 */
void extractWeights(GraphProto& graph, arma::mat& weightMatrix)
{
  auto& weights = graph.initializer();
  int totalWeights = 0;
  int weightDims[weights.size()];
  int wtNumber = 0;
  for(TensorProto weight:weights)
  {
    if (weight.dims().size() == 0)
        continue;
    int weightSize = weight.dims(0);
    for (int i = 1; i < weight.dims_size(); ++i)
    {
      weightSize *= weight.dims(i);
    }
    weightDims[wtNumber++] = weightSize;
    totalWeights += weightSize;
  }
  wtNumber = 0;// reinitialize for use
  int count = 0;
  weightMatrix.set_size(totalWeights, 1);
  for(TensorProto weight:weights)
  {
    if (weight.dims().size() == 0)
      continue;
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
    wtNumber++;
  }
}
