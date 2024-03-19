#include "onnx_pb.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using namespace onnx;

NodeProto nextNode(const GraphProto &graph, string preNodeOutput);

string modelInput(const GraphProto &graph)
{
    vector<string> inputNames;
    vector<string> initializerNames;
    for (auto input : graph.input())
    {
        inputNames.push_back(input.name());
    }
    for (auto initializer : graph.initializer())
    {
        initializerNames.push_back(initializer.name());
    }

    for (const auto &element : inputNames)
    {
        if (std::find(initializerNames.begin(), initializerNames.end(), element) == initializerNames.end())
        {
            return element;
        }
    }
    return "all elements found";
}


NodeProto nextNode(const GraphProto &graph, string preNodeOutput)
{
    NodeProto node;

    for (auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr)
    {
        for (int i = 0; i < nodeItr->input().size(); i++)
        {
            if (preNodeOutput == nodeItr->input(i))
            {
                return *nodeItr;
            }
        }
    }
    return node;
}


int findOutputDimension(const GraphProto &graph, const NodeProto &node)
{
  int dim;
  string layerOutput = node.output(0);
  NodeProto nextNod = nextNode(graph, layerOutput);
  for (auto initializer : graph.initializer())
  {
    if(initializer.name() == nextNod.input(1))
    {
        dim = initializer.dims(1);
    }
  }
  return dim;
}





void getLayer(GraphProto graph, const NodeProto &node, string layerType)
{
    // keys => ONNX operator name
    // values => mlpack operator name
    map<string, string> operatorMap = {
        {"Conv", "convolution"},
        {"Reshape", ""},
        {"MaxPool", "maxpooling"},
        {"MatMul", "linearnobias"},
        {"Add", "add"},
        {"Relu", "relu"},
        {"Softmax", "softmax"},
        {"Identity", "identity"}};

    // we are trying to get these many informatoin from
    // the onnx for conv layer
    map<string, map<string, vector<string>>> onnxAttributes;
    onnxAttributes["Conv"] = {
        {"kernel_shape", {"kh", "kw"}},
        {"pads", {"padh", "padw"}},
        {"strides", {"dh", "dw"}}};
    onnxAttributes["MaxPool"] = {
        {"kernel_shape", {"kh", "kw"}},
        {"strides", {"dh", "dw"}}
    };
    onnxAttributes["MatMul"];
    onnxAttributes["LeakyRelu"] = {
        {"alpha", {"alpha"}}
    };
    onnxAttributes["Identity"];
    onnxAttributes["Relu"];


    map<string, vector<string>> onnxLayerAttributes = onnxAttributes[layerType];
    map<string, double> mlpackLayerAttribute;
    if(node.op_type() == "MatMul")
    {
        mlpackLayerAttribute["outSize"] = findOutputDimension(graph, node);

    }
    // stores the attributes which can be calculated
    // only after knowing the other ones
    // vector<string> skippedAttributes;
    for (AttributeProto attribute : node.attribute())
    {
        string attrName = attribute.name();
        // if present then it will choose that other wise it will
        // make the new one
        // but still it can't iterate through it
        vector<string> attr = onnxLayerAttributes[attrName];
        vector<string>::iterator itr;

        // checking for special cases
        if (attrName == "pads")
        {
            //[0 1 2 3] indices are top, bottom, left, right respectively
            mlpackLayerAttribute["padw"] = (int)(attribute.ints(1) + attribute.ints(3)) / 2;
            mlpackLayerAttribute["padh"] = (int)(attribute.ints(0) + attribute.ints(2)) / 2;
        }
        // else if (attrName == "auto_pad")
        // {
        //     // P = ((S-1)*W-S+F)/2
        //     // It calculates symmetric padding, meaning the same amount of padding is added on both sides.
        //     skippedAttributes.push_back("auto_pad_" + attribute.s());   //will be like auto_pad_sameupper
        // }
        else if (attrName == "auto_pad")
        {
            if (attribute.s() == "SAME_UPPER" || attribute.s() == "SAME_LOWER")
            {
                mlpackLayerAttribute["paddingType"] = 0;  //same
            }
            else if (attribute.s() == "VALID")
            {
                mlpackLayerAttribute["paddingType"] = 1;  //valid
            }
            else if (attribute.s() == "NOTSET")
            {
                mlpackLayerAttribute["paddingType"] = 2;  //none
            }
        }

        int i = 0;
        for (itr = attr.begin(); itr < attr.end(); ++itr, ++i)
        {
            if (attribute.type() == attribute.INT)
            {
                mlpackLayerAttribute[*itr] = attribute.i();
            }
            else if (attribute.type() == attribute.INTS)
            {
                mlpackLayerAttribute[*itr] = attribute.ints()[i];
            }
            else if (attribute.type() == attribute.FLOAT)
            {
                mlpackLayerAttribute[*itr] = attribute.f();
            }
            else if (attribute.type() == attribute.FLOATS)
            {
                mlpackLayerAttribute[*itr] = attribute.floats()[i];
            }
        }
    }

    // while passing the onnx attribute to the getrefrence we will also
    // pass the dimensional parameters
    map<string, double>::iterator itr;
    // printing the map
    for (auto element : mlpackLayerAttribute)
    {
        cout << element.first << " " << element.second << endl;
    }
}












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

    string layerIn = modelInput(graph);

    while (nextNode(graph, layerIn).op_type() != "")
    {
        NodeProto node = nextNode(graph, layerIn);
        cout<<node.op_type()<<"---------------------"<<endl;
        getLayer(graph, node, node.op_type());
        cout<<"\n\n"<<endl;

        layerIn = node.output(0);
    }



    return 0;
}