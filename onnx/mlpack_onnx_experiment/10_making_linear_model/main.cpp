#include "onnx_pb.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <map>
#include <string>
#include "mlpack.hpp"

using namespace onnx;
using namespace mlpack;
using namespace ann;
using namespace std;

// FFN<> generateModel(GraphProto& graph)
// {
//   FFN<> ffn;

//   std::vector<int> dims = findWeightDims(graph.initializer());
//   std::vector<int>::iterator itr = dims.begin();
//   for (auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr)
//   {
//     string nodeType = nodeItr -> op_type();
//     map<string, double> dimParams;
//     std::vector<string> skipLayers = {"Add", "Identity", "Reshape", "Transpose", "Unsqueeze",
//     "Shape", "Cast", "Slice", "Concat", "ReduceProd"};
//     std::vector<string> dimensionalLayers = {"ConstantOfShape", "Conv", "Gemm", "Matmul",
//     "ConvTranspose", "Transformed_linear"};// may be an error with the ConstantOfShape layer
//     // layers for merging, currently just one is there
//     std::map<std::vector<string>, string> mergeLayers = {
//         {{"MatMul", "Add"}, "Transformed_linear"}
//         };
//     std::map<std::vector<string>, string>::iterator mergeItr;
//     for (mergeItr = mergeLayers.begin(); mergeItr != mergeLayers.end(); ++mergeItr)
//     {
//       std::vector<string> mergeVector = mergeItr -> first;
//       int i;
//       for (i = 0; i < mergeVector.size(); ++i)
//       {
//         //cout << "i= " << i << " mervector[i]= " << mergeVector[i] << " op_type= " << (nodeItr + i) -> op_type() << "\n";
//         if ((nodeItr + i) == std::end(graph.node()) || mergeVector[i] != (nodeItr + i) -> op_type())
//           break;
//       }
//       //cout << "i= " << i << " mergeVector.size()= " << mergeVector.size() << "\n";
//       if (i == mergeVector.size())
//       {
//      mespace ann;
// using namespace std;   nodeItr += i;
//         nodeType = mergeItr -> second;
//       }
//     }

//     if (std::find(skipLayers.begin(), skipLayers.end(), nodeType) != skipLayers.end())
//       continue;

//     if (std::find(dimensionalLayers.begin(), dimensionalLayers.end(), nodeType) != dimensionalLayers.end())
//     {
//       dimParams["insize"] = *itr;
//       itr++;
//       dimParams["outsize"] = *itr;
//     }
//     if(nodeType == "Conv")
//     {
//       dimParams["inputwidth"] = storedParams["inputwidth"];
//       dimParams["inputheight"] = storedParams["inputheight"];
//     }
//     std::cout << "Node type: " << nodeType << "\n";
//     printMap(dimParams);
//     ffn.Add(getLayer(*nodeItr, nodeType, dimParams));
//   }
//   return ffn;
// }

FFN<> generateModel(GraphProto &graph)
{
    FFN<> ffn;
    int inputShape = graph.input(0).type().tensor_type().shape().dim(1).dim_value();
    int outputShape = graph.output(0).type().tensor_type().shape().dim(1).dim_value();
    cout << "\ninput shape :: " << inputShape << " output shape ::" << outputShape << endl;

    for (auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr)
    {
        // temporarly storing the nodeType
        string nodeType = nodeItr->op_type();
        // each node in the graph do some operation, but there are
        // some operation that onnx persorm in two steps but mlpack
        // perform only in onestep so we have to merge those nodes
        // whose opertions can be done in only one step in mlpack
        // current we are adding only one such case
        std::map<std::vector<string>, string> mergeLayers = {
            {{"MatMul", "Add"}, "Transformed_linear"}};
        // Iterating through the mergerLayer
        for (auto iter = mergeLayers.begin(); iter != mergeLayers.end(); ++iter)
        {
            std::vector<std::string> mergeVector = iter->first;
            if (nodeItr->op_type() == mergeVector[0] && (nodeItr + 1)->op_type() == mergeVector[1])
            {
                nodeItr += 1;
                nodeType = iter->second;
            }
        }
        cout << "layer:: " << nodeType << endl;
    }

    return ffn;
}


LayerTypes<> getLayer(const NodeProto& node, string layerType,
                    map<string, double>& dimParams)
{



int main()
{
    onnx::ModelProto onnxModel;
    std::ifstream in("onnx_linear_model.onnx", std::ios_base::binary);
    onnxModel.ParseFromIstream(&in);
    in.close();

    // graph will contains the information of
    //  node, initializer, input, output
    onnx::GraphProto graph = onnxModel.graph();

    FFN<> ffn = generateModel(graph);

    return 0;
}