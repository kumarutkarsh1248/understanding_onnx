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
    std::ifstream in("linear_regression.onnx", std::ios_base::binary);
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
    cout<<graph.DebugString()<<endl;
    cout<<graph.initializer().debugstring()<<endl;

    // cout<<"the size fo the graph is "<<graph.initializer_size()<<endl;
    // for (int i =0; i<graph.initializer_size(); i++)
    // {
    //     cout<< graph.mutable_initializer(i)->DebugString() <<endl;
    // }
    // cout<<"done"<<endl;




    return 0;
}