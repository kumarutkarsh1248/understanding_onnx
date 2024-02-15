#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template<typename T>
void print_vector(std::vector<T> v)
{
    for(auto element : v)
    {
        cout<<element<<" ";
    }
    cout<<endl;
}


int main() {
    // Create a float array to represent the NumPy tensor
    float data[] = {0, 1, 4, 5, 3};
    std::vector<float> tensor_data(data, data + 5);

    cout<<"this is your tensor "<<endl;
    print_vector(tensor_data);

    // Create ONNX tensor
    onnx::TensorProto tensor_proto;
    tensor_proto.set_name("my_tensor");  // Set a name for the tensor
    tensor_proto.set_data_type(onnx::TensorProto::FLOAT);
    for (float value : tensor_data) {
      tensor_proto.add_float_data(value);
    }
    tensor_proto.mutable_dims()->Add(tensor_data.size());  // Set the dimension

    // Serialize to string
    std::string serialized_tensor = tensor_proto.SerializeAsString();

    // Save to file
    std::ofstream file("saved_tensor.pb", std::ios::out | std::ios::binary);
    file.write(serialized_tensor.c_str(), serialized_tensor.size());
    file.close();

    std::cout << "Tensor saved successfully!" << std::endl;




    //--------------loading the binary file and grabing the values
    onnx::ModelProto onnxModel;
    std::ifstream in("saved_tensor.pb", std::ios_base::binary);
    onnxModel.ParseFromIstream(&in);
    in.close();
    

    return 0;
}
