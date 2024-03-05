#define MLPACK_PRINT_DEBUG
#define MLPACK_PRINT_INFO
#define MLPACK_PRINT_WARN


#include "onnx_pb.h"
#include <fstream>
#include <iostream>
#include <memory>
#include<map>
#include <string>
#include "mlpack.hpp"


// #include "model_parser.hpp"

using namespace onnx;
using namespace mlpack;
using namespace ann;
using namespace std;

map<string, double> storedParams;



void printMap(std::map<std::string, double> params)
{
  std::map<std::string, double>::iterator itr;
  for (itr = params.begin(); itr != params.end(); ++itr)
  {
    Log::Info << itr->first << " : " << itr->second << "\n";
  }
}



void printVector(vector<string> v)
{
  cout<<"vector::";
  for(auto element:v)
  {
    
    cout<<element<<" ";
  }
  cout<<endl;
}











/**
 * @param layerType => onnx operator
 * @param dimparams => map where you want to store the information
*/

void getLayer(const NodeProto& node, string layerType, map<string, double>& dimParams)
{

  //atlast we are printing this mappedParams
  map<string, double> mappedParams;


  
  map<string, string> operatorMap;
  // keys are onnx operators, values are the corresponding mlpack layer names
  //https://github.com/onnx/onnx/blob/main/docs/Operators.md
  // {onnx_operator, mlpack_opertor}
  operatorMap = {
      {"BatchNormalization", "batchnorm"},
      {"ConstantOfShape", "constant"},// probably this and not Constant
      {"Conv", "convolution"},
      {"Dropout", "dropout"},
      {"LeakyRelu", "leakyrelu"},
      {"Transformed_linear", "linear"},
      {"Gemm", "linear"},// needs validation
      {"MatMul", "linearnobias"},
      {"LogSoftmax", "logsoftmax"},
      {"ConvTranspose", "transposedconvolution"},
      {"Elu", "elu"},
      {"MaxPool", "maxpooling"},
      {"Identity", "identity"},
      {"PRelu", "prelu"},
      {"Relu", "relu"},
      {"Selu", "selu"},
      {"Sigmoid", "sigmoid"},
      {"Softmax", "softmax"},
      {"LogSoftMax", "logsoftmax"},
      //{"Softsign", "softsign"},// Needs to be defined in the parser
      {"Tanh", "tanh"}
  };

  // {string, {string, vector}}
  map<string, map<string, vector<string> > > mappedNames;
  //mappedNames["BatchNormalization"]; // Needs fixing

  //17 mapped names has already been defined
  //keys are according to the onnx convention
  mappedNames["Conv"] = {
      //string, vector
      {"kernel_shape", {"kh", "kw"}},
      {"pads", {"padh", "padw"}},
      {"strides", {"dh", "dw"}}
  };

  mappedNames["Dropout"] = {
      {"ratio", {"ratio"}}
  };

  mappedNames["LeakyRelu"] = {
      {"alpha", {"alpha"}}
  };

  mappedNames["Gemm"];

  mappedNames["MatMul"];

  mappedNames["LogSoftmax"];

  mappedNames["ConvTranspose"] = {
      {"kernel_shape", {"kh", "kw"}},
      {"pads", {"padh", "padw"}},
      {"strides", {"dh", "dw"}}
  };

  mappedNames["Elu"] = {
      {"alpha", {"alpha"}}
  };

  mappedNames["MaxPool"] = {
      {"kernel_shape", {"kh", "kw"}},
      {"strides", {"dh", "dw"}}
  };// support for 'pads' missing in mlpack

  mappedNames["Identity"];

  mappedNames["PRelu"] = {
      {"slope", {"alpha"}}
  };// 'slope' is actually not an attribute rather an input

  mappedNames["Relu"];

  mappedNames["Selu"];// support for custom alpha or gamma missing in mlpack

  mappedNames["Sigmoid"];

  mappedNames["Softmax"];// not yet implemented in mlpack

  mappedNames["Softsign"];// not yet implemented in the parser

  mappedNames["Tanh"];




  //now we know the layer and we want to get the info regarding that layer
  map<string, vector<string>> layer = mappedNames[layerType];


  /*stores the attributes which can be calculated
  only after knowing the other ones*/
  vector<string> skippedAttributes;

  // node {
  //   input: "Block386_Output_0"
  //   input: "Constant321"
  //   output: "Convolution396_Output_0"
  //   name: "Convolution28"
  //   op_type: "Conv"
  //   attribute {
  //     name: "kernel_shape"
  //     ints: 5
  //     ints: 5
  //     type: INTS
  //   }
  //   attribute {
  //     name: "strides"
  //     ints: 1
  //     ints: 1
  //     type: INTS
  //   }
  //   attribute {
  //     name: "auto_pad"
  //     s: "SAME_UPPER"
  //     type: STRING
  //   }
  //   attribute {
  //     name: "group"
  //     i: 1
  //     type: INT
  //   }
  //   attribute {
  //     name: "dilations"
  //     ints: 1
  //     ints: 1
  //     type: INTS
  //   }
  //   doc_string: ""
  //   domain: ""
  // }
  


  //now we will move throught each attribute of the node
  for (AttributeProto attribute:node.attribute())
  {
    string attrName = attribute.name();

    cout<<attrName<<" attribute_type "<< attribute.type()<<" attribute_INT "<< attribute.INT<<endl;
    cout<<" attribute_INT "<< attribute.INT<<endl;
    cout<<" attribute_INTS "<< attribute.INTS<<endl;
    cout<<" attribute_FLOAT "<< attribute.FLOAT<<endl;
    cout<<" attribute_FLOATS "<< attribute.FLOATS<<endl;
    cout<<" attribute_ITENSOR "<< attribute.TENSOR<<endl;
    cout<<" attribute_TENSORS "<< attribute.TENSORS<<endl;
    //like layer["kernel_shape"] => {"kh", "kw"}
    vector<string> attr = layer[attrName];
    // printVector(attr);

    vector<string>::iterator itr;
    //check for special cases
    // onnx provide 4 values for padding but mlpack require only two padh, padw



    if (attrName == "pads")
    {
      // [0 1 2 3] indices are top, bottom, left, right respectively
      mappedParams["padw"] = (int) (attribute.ints()[1] + attribute.ints()[3]) / 2;
      mappedParams["padh"] = (int) (attribute.ints()[0] + attribute.ints()[2]) / 2;
      continue;
    }
    else if (attrName == "auto_pad")
    {
    //   P = ((S-1)*W-S+F)/2
      // It calculates symmetric padding, meaning the same amount of padding is added on both sides.
      skippedAttributes.push_back("auto_pad_" + attribute.s());   //will be like auto_pad_sameupper
    }






    int i = 0;
    // validation needs to be added
    for (itr = attr.begin(); itr < attr.end(); ++itr, ++i)
    {
      if (attribute.type() == attribute.INT)
      {
        mappedParams[*itr] = attribute.i();
      }
      else if (attribute.type() == attribute.INTS)
      {
        mappedParams[*itr] = attribute.ints()[i];
      }
      else if (attribute.type() == attribute.FLOAT)
      {
        mappedParams[*itr] = attribute.f();
      }
      else if (attribute.type() == attribute.FLOATS)
      {
        mappedParams[*itr] = attribute.floats()[i];
      }
    }
  }






  map<string, double>::iterator itr;
  for (itr = dimParams.begin(); itr != dimParams.end(); ++itr)
  {
    mappedParams[itr->first] = itr->second;
  }


  for (string& attribute:skippedAttributes)
  {
    if (attribute ==  "auto_pad_SAME")
    {
      mappedParams["padw"] = (int) ((mappedParams["inputwidth"] * (mappedParams["dw"] - 1) - mappedParams["dw"] + mappedParams["kw"] + 1) / 2);
      mappedParams["padh"] = (int) ((mappedParams["inputheight"] * (mappedParams["dh"] - 1) - mappedParams["dh"] + mappedParams["kh"] + 1) / 2);
    }
    else if (attribute == "auto_pad_VALID") // not absolutely necessary though as the default is zero pad
    {
      mappedParams["padw"] = 0;
      mappedParams["padh"] = 0;
    }
  }


  // store dimensional details for the next layer if needed
  if(layerType == "Conv")
  {
    storedParams["inputwidth"] = (mappedParams["inputwidth"] - mappedParams["kw"] + 2 * mappedParams["padw"]) / mappedParams["dw"] + 1;
    storedParams["inputheight"] = (mappedParams["inputheight"] - mappedParams["kh"] + 2 * mappedParams["padh"]) / mappedParams["dh"] + 1;
  }
  cout << "Layer type of mlpack model: " << operatorMap[layerType] << "\nLayer map:\n";
  printMap(mappedParams);
  cout << "\n\n";


  // return getNetworkReference(operatorMap[layerType], mappedParams);
}






int main()
{

    onnx::ModelProto onnxModel;
    std::ifstream in("feastconv_Opset18.onnx", std::ios_base::binary);
    onnxModel.ParseFromIstream(&in);
    in.close();

    // map<string, double> dim_param;
    onnx::GraphProto graph = onnxModel.graph();


    cout<<graph.initializer(0).DebugString()<<endl;


    // // Iterate through all nodes in the graph
    // for (const onnx::NodeProto& node : graph.node()) {


    //     cout<<node.op_type()<<"***********"<<endl;
    //     getLayer(node, "Conv", dim_param);
    //     cout<<"\n\n"<<endl;
    //     break;
    // }


  // onnx::AttributeProto att;
  // cout<<att.FLOAT<<endl; //1
  // cout<<att.INT<<endl; //2
  // cout<<att.STRING<<endl; //3
  // cout<<att.TENSOR<<endl; //4
  // cout<<att.FLOATS<<endl;//6
  // cout<<att.INTS<<endl; //7
  // cout<<att.STRINGS<<endl; //8
  // cout<<att.TENSORS<<endl; //9


  // onnx::AttributeProto att;
  // cout<<onnx::AttributeProto::FLOAT<<endl; //1
  // cout<<onnx::AttributeProto::INT<<endl; //2
  // cout<<onnx::AttributeProto::STRING<<endl; //3
  // cout<<onnx::AttributeProto::TENSOR<<endl; //4
  // cout<<onnx::AttributeProto::FLOATS<<endl;//6
  // cout<<onnx::AttributeProto::INTS<<endl; //7
  // cout<<onnx::AttributeProto::STRINGS<<endl; //8
  // cout<<onnx::AttributeProto::TENSORS<<endl; //9




    return 0;
}