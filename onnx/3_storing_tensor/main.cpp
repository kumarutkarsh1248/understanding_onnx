#include <onnx/onnx_pb.h>
#include <iostream>

int main() {
    // Create a TensorProto object
    onnx::TensorProto tensor;

    // Set data type (e.g., float)
    tensor.set_data_type(onnx::TensorProto_DataType_FLOAT);

    // Set dimensions (e.g., 2x2)
    tensor.add_dims(2);
    tensor.add_dims(2);

    // Set tensor values
    tensor.add_float_data(1.0);
    tensor.add_float_data(2.0);
    tensor.add_float_data(3.0);
    tensor.add_float_data(4.0);

    // Print the tensor for verification
    std::cout << "TensorProto:\n" << tensor.DebugString() << std::endl;

    // Now, you can add this tensor to your ONNX model
    // For example, if you have a GraphProto object representing your model,
    // you can add this tensor to the initializer list or as an input/output of a node.

    return 0;
}
