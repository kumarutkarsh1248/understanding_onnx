#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void func(const onnx::NodeProto &node)
{
    auto nextNodeItr = &node; // Move to the next node in the sequence
    cout << "node info " << nextNodeItr->op_type() << endl;
}


int main()
{
    /*
    ModelProto is a C++ class that serves as the top-level structure for representing an entire machine learning model
    */
    onnx::ModelProto onnxModel;
    std::ifstream in("onnx_linear_model.onnx", std::ios_base::binary);
 
    onnxModel.ParseFromIstream(&in);
    in.close();

    onnx::GraphProto graph = onnxModel.graph();
 
    for( auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr )
    {

        auto nextNodeItr = &(*nodeItr) + 1; // Move to the next node in the sequence
        cout << "node info " << (*nextNodeItr).DebugString() << endl; 
        break;
    }





    return 0;
}