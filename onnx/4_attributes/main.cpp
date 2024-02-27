#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


int main()
{
    onnx::ModelProto onnxModel;
    std::ifstream in("mnist.onnx", std::ios_base::binary);
    
    onnxModel.ParseFromIstream(&in);
    in.close();

    onnx::GraphProto graph = onnxModel.graph();

    for (const onnx::NodeProto& node : graph.node()) {
        cout<<"\n\n"<<endl;

        // Access information about each node
        // std::string nodeName = node.name();
        std::string opType = node.op_type();
        // std::cout << "Node Name: " << nodeName << ", Operator Type: " << opType << std::endl;
        std::cout <<"Operator Type: " << opType << std::endl;

    //     // Access inputs and outputs of the node
    //     for (const std::string& input : node.input()) {
    //         std::cout << "Input: " << input << std::endl;
    //     }
    //     for (const std::string& output : node.output()) {
    //         std::cout << "Output: " << output << std::endl;
    //     }

    //     // Access attributes of the node
    //     for (const onnx::AttributeProto& attr : node.attribute()) {
    //         std::string attrName = attr.name();
    //         std::cout << "Attribute Name: " << attrName << std::endl;
    //     }
    }

}