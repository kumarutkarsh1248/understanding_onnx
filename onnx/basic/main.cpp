#include <iostream>
#include "onnx_pb.h"

int main() {
    // Create inputs
    onnx::TypeProto floatType;
    floatType.mutable_tensor_type()->set_elem_type(onnx::TensorProto::FLOAT);
    floatType.mutable_tensor_type()->mutable_shape()->add_dim();
    floatType.mutable_tensor_type()->mutable_shape()->add_dim();

    onnx::ValueInfoProto X, A, B, Y;
    X.set_name("X");
    *X.mutable_type() = floatType;

    A.set_name("A");
    *A.mutable_type() = floatType;

    B.set_name("B");
    *B.mutable_type() = floatType;

    Y.set_name("Y");
    *Y.mutable_type() = floatType;

    // Create nodes
    onnx::NodeProto node1, node2;
    node1.set_op_type("MatMul");
    node1.add_input("X");
    node1.add_input("A");
    node1.add_output("XA");

    node2.set_op_type("Add");
    node2.add_input("XA");
    node2.add_input("B");
    node2.add_output("Y");

    // Create graph
    onnx::GraphProto graph;
    *graph.add_input() = X;
    *graph.add_input() = A;
    *graph.add_input() = B;
    *graph.add_output() = Y;
    *graph.add_node() = node1;
    *graph.add_node() = node2;

    // Create ONNX model
    onnx::ModelProto onnxModel;
    onnxModel.set_allocated_graph(&graph);

    // Check model consistency
    std::string errorString;
    if (!onnx::checker::check_model(onnxModel, &errorString)) {
        std::cerr << "Error: " << errorString << std::endl;
        return 1;
    }

    // Print the ONNX model
    std::cout << onnxModel.DebugString() << std::endl;

    return 0;
}
