#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace onnx;

vector<int> findInputDimension(GraphProto graph, string input_string)
{   
    vector<int> dimension;
    for (auto input : graph.input())
    {
        if(input.name() == input_string)
        {
            int dim_size = input.type().tensor_type().shape().dim().size();
            for (int i = 1; i < dim_size; i++)
            {
                dimension.push_back(input.type().tensor_type().shape().dim(i).dim_value());
            }
        }
    }
    return dimension;
}

string modelInput(const GraphProto &graph)
{
  vector<string> inputNames;
  vector<string> initializerNames;
    for (auto input : graph.input())
    {
        inputNames.push_back(input.name());
    }
    for (auto initializer : graph.initializer())
    {
        initializerNames.push_back(initializer.name());
    }

    for (const auto& element : inputNames) {
        if (std::find(initializerNames.begin(), initializerNames.end(), element) == initializerNames.end()) {
          return element;
        }
    }
    return "all elements found";
}

int main()
{
    /*
    ModelProto is a C++ class that serves as the top-level structure for representing an entire machine learning model
    */
    onnx::ModelProto onnxModel;
    std::ifstream in("mnist_model.onnx", std::ios_base::binary);
    onnxModel.ParseFromIstream(&in);
    in.close();
    onnx::GraphProto graph = onnxModel.graph();

    vector<int> v = findInputDimension(graph, modelInput(graph));
    for(int element:v)
    {
        cout<<element<<endl;
    }

    return 0;
}