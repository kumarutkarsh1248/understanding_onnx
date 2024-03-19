#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


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

    vector<string> input_names;
    vector<string> initializer_names;
    for (auto input : graph.input())
    {
        input_names.push_back(input.name());
    }
    for (auto initializer : graph.initializer())
    {
        input_names.push_back(initializer.name());
    }

    for (const auto& element : input_names) {
        if (std::find(initializer_names.begin(), initializer_names.end(), element) == initializer_names.end()) {
            std::cout << "First element of B not in A: " << element << std::endl;
            break; // Stop after finding the first element
        }
    }

    return 0;
}