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
    std::ifstream in("linear_regression.onnx", std::ios_base::binary);
 
    onnxModel.ParseFromIstream(&in);
    in.close();

    string myString = onnxModel.DebugString();
    
    // Specify the file name you want to write to
    std::string filename = "linear_regression.txt";
    
    // Create an ofstream object and open the file
    std::ofstream outputFile(filename);
    
    // Check if the file is opened successfully
    if (outputFile.is_open()) {
        // Write the string to the file
        outputFile << myString;
        
        // Close the file
        outputFile.close();
        
        std::cout << "String saved to file successfully." << std::endl;
    } else {
        std::cerr << "Error opening file." << std::endl;
    }

    onnx::GraphProto graph = onnxModel.graph();
 
    for( auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr )
    {

        cout << "node info " << nodeItr->name() << endl; 
    }





    return 0;
}