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
    std::ifstream in("mnist.onnx", std::ios_base::binary);
    // if(in.empty())
    // {
    //     cout<<"the file is empty"<<endl;
    //     return -1;
    // }
    

    /*
    Parses the data from an input stream and attempts to interpret it as a valid ONNX model.
    If successful, it populates an onnx::ModelProto object with the loaded model information (metadata, graph, tensors, etc.).
    */
    onnxModel.ParseFromIstream(&in);
    in.close();

    cout<<"***********************************"<<endl;
    cout<<onnxModel.DebugString()<<endl;
    cout<<"***********************************"<<endl;

    /*
    onnx graph has all the information regarding input, output
    nodes but i think it has no information regarding the initializer
    i.e weight values
    */
    onnx::GraphProto graph = onnxModel.graph();
    // cout<<graph.DebugString()<<endl;
    // cout<<onnxModel.graph().node().DebugString()<<endl;

    // cout<<"the size fo the graph is "<<graph.initializer_size()<<endl;
    
    



    // Iterate through all nodes in the graph
    for (const onnx::NodeProto& node : graph.node()) {
        cout<<"\n\n"<<endl;
        // Access information about each node
        std::string nodeName = node.name();
        std::string opType = node.op_type();
        std::cout << "Node Name: " << nodeName << ", Operator Type: " << opType << std::endl;

        // Access inputs and outputs of the node
        for (const std::string& input : node.input()) {
            std::cout << "Input: " << input << std::endl;
        }
        for (const std::string& output : node.output()) {
            std::cout << "Output: " << output << std::endl;
        }

        // Access attributes of the node
        for (const onnx::AttributeProto& attr : node.attribute()) {
            std::string attrName = attr.name();
            std::cout << "Attribute Name: " << attrName << std::endl;
            // Handle different types of attributes accordingly
            // For example:
            // if (attr.type() == AttributeProto_AttributeType_INT) {
            //     int attrValue = attr.i();
            //     std::cout << "Attribute Value (INT): " << attrValue << std::endl;
            // }
        }
    }





    return 0;
}