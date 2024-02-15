#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


int main()
{
    onnx::ModelProto onnxModel;
    std::ifstream in("linear_regression.onnx", std::ios_base::binary);
    // if(in.empty())
    // {
    //     cout<<"the file is empty"<<endl;
    //     return -1;
    // }
    onnxModel.ParseFromIstream(&in);
    in.close();

    cout<<"***********************************"<<endl;
    cout<<onnxModel.DebugString()<<endl;
    cout<<"***********************************"<<endl;


    onnx::GraphProto graph = onnxModel.graph();
    // cout<<graph.DebugString()<<endl;
    // cout<<graph.initializer().debugstring()<<endl;

    cout<<"the size fo the graph is "<<graph.initializer_size()<<endl;
    for (int i =0; i<graph.initializer_size(); i++)
    {
        cout<< graph.mutable_initializer(i)->DebugString() <<endl;
    }
    cout<<"done"<<endl;




    return 0;
}