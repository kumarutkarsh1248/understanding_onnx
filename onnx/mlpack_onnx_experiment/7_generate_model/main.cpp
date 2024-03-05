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
#include <armadillo>



// #include "model_parser.hpp"

using namespace onnx;
using namespace mlpack;
using namespace ann;
using namespace std;

map<string, double> storedParams;


/**
 * Get the input size of each layer and the output size of the last layer in
 * a vector
 *
 * @param weights The data structure containing the weights and biases
 * of the ONNX model
 * @return The number of nodes in each layer
 */
std::vector<int> findWeightDims
// const ::google::protobuf::RepeatedPtrField => when multiple repeated message is to be hold
(const ::google::protobuf::RepeatedPtrField< ::onnx::TensorProto>& weights)
{

  std::vector<int> dims;
  auto itr = std::begin(weights);

  //*itr => first tenserproto
  //dim is repeated type
  if ((*itr).dims().size() > 2)
  {
    dims.push_back((*itr).dims(1));
    dims.push_back((*itr).dims(0));
    itr += 2; //may be after weight we would have bias term so +=2
  }

  
  for (; itr != std::end(weights); itr += 2)
  {
    if ((*itr).dims().size() == 0)
    {
      itr--;
    }
    else
    {
      dims.push_back((*itr).dims(0));
    }
  }
  itr -= 2;
  dims.push_back((*itr).dims(1));
  return dims;
}



void printMap(std::map<std::string, double> params)
{
  std::map<std::string, double>::iterator itr;
  for (itr = params.begin(); itr != params.end(); ++itr)
  {
    Log::Info << itr->first << " : " << itr->second << "\n";
  }
}














void updateParams(std::map<std::string, double> &origParams,
                  std::map<std::string, double> &newParams)
{
  std::map<std::string, double>::iterator itr;
  bool error = false;
  for (itr = origParams.begin(); itr != origParams.end(); ++itr)
  {
    std::map<std::string, double>::iterator itr2 = newParams.find(itr->first);
    if (itr2 == newParams.end() && isnan(itr->second))
    {
      Log::Info << "Required parameter: " << itr->first << "\n";
      error = true;
    }
    else if (itr2 != newParams.end())
      itr->second = newParams.at(itr->first);
  }
  if (error)
    Log::Fatal << "Required parameters missing" << "\n";
}


std::string decodePadType(double val)
{
  if (val==0)
    return "None";
  else if (val==1)
    return "Valid";
  else
    return "Same";
}




/**
 * Determine the layer type to be added to a feedforward network given a
 * string containing the type and a map containing the parameters
 *
 * @param layerType Type of layer that is to be defined
 * @param layerParams Map containing the parameters of the layer to be defined
 * @return A LayerTypes<> object that is of the given type and is
 * initialized by the given parameters
 */
Layer<> *getNetworkReference(std::string& layerType,
                                 std::map<std::string, double>& layerParams)
{

  std::map<std::string, double> origParams;
  Layer<> *layer;

//   if (layerType == "atrousconvolution")
//   {
//     origParams["insize"] = NAN;
//     origParams["outsize"] = NAN;
//     origParams["kw"] = NAN;
//     origParams["kh"] = NAN;
//     origParams["dw"] = 1;
//     origParams["dh"] = 1;
//     origParams["padw"] = 0;
//     origParams["padh"] = 0;
//     origParams["inputwidth"] = 0;
//     origParams["inputheight"] = 0;
//     origParams["dilationw"] = 1;
//     origParams["dilationh"] = 1;
//     origParams["paddingtype"] = 0; // None = 0 , Valid = 1, Same = 2
//     updateParams(origParams, layerParams);
//     std::string padding = decodePadType(origParams["paddingtype"]);
//     layer = new AtrousConvolution<>(origParams["insize"],
//         origParams["outsize"], origParams["kw"], origParams["kh"],
//         origParams["dw"], origParams["dh"], origParams["padw"],
//         origParams["padh"], origParams["inputwidth"],
//         origParams["inputheight"], origParams["dilationw"],
//         origParams["dilationh"], padding);
//   }


//   else if (layerType == "alphadropout")
//   {
//     origParams["ratio"] = 0.5;
//     // alphadash is the default value of -alpha*lambda
//     origParams["alphadash"] = -1.758099340847376624;
//     updateParams(origParams, layerParams);
//     layer = new AlphaDropout<>(origParams["ratio"], origParams["alphadash"]);
//   }
  // else if (layerType == "batchnorm")
  // {
  //   // Add the commented part after next mlpack release.
  //   origParams["size"] = NAN;
  //   origParams["eps"] = 1e-8;
  //   //origParams["average"] = 1;
  //   //origParams["momentum"] = 0.1;
  //   updateParams(origParams, layerParams);
  //   // layer = new BatchNormType<>(origParams["size"], origParams["eps"]);
  //   layer = new BatchNormType<>(2);
  //   //,
  //       //origParams["average"], origParams["momentum"]);
  // }
//   else if (layerType == "constant")
//   {
//     origParams["outsize"] = NAN;
//     origParams["scalar"] = 0.0;
//     layer = new Constant<>(origParams["outsize"], origParams["scalar"]);
//   }
//   if (layerType == "convolution")
//   {
//     origParams["insize"] = NAN;
//     origParams["outsize"] = NAN;
//     origParams["kw"] = NAN;
//     origParams["kh"] = NAN;
//     origParams["dw"] = 1;
//     origParams["dh"] = 1;
//     origParams["padw"] = 0;
//     origParams["padh"] = 0;
//     origParams["inputwidth"] = 0;
//     origParams["inputheight"] = 0;
//     origParams["paddingtype"] = 0; // None = 0 , Valid = 1, Same = 2
//     updateParams(origParams, layerParams);
//     std::string padding = decodePadType(origParams["paddingtype"]);
//     layer = new Convolution<>(origParams["insize"], origParams["outsize"],
//         origParams["kw"], origParams["kh"], origParams["dw"],
//         origParams["dh"], origParams["padw"], origParams["padh"],
//         origParams["inputwidth"], origParams["inputheight"], padding);
//   }
//   else if (layerType == "dropconnect")
//   {
//     origParams["insize"] = NAN;
//     origParams["outsize"] = NAN;
//     origParams["ratio"] = 0.5;
//     updateParams(origParams, layerParams);
//     layer = new DropConnect<>(origParams["insize"], origParams["outsize"],
//                               origParams["ratio"]);
//   }
//   else if (layerType == "dropout")
//   {
//     origParams["ratio"] = 0.5;
//     updateParams(origParams, layerParams);
//     layer = new Dropout<>(origParams["ratio"]);
//   }
//   else if (layerType == "layernorm")
//   {
//     origParams["size"] = NAN;
//     origParams["eps"] = 1e-8;
//     updateParams(origParams, layerParams);
//     layer = new LayerNorm<>(origParams["size"], origParams["eps"]);
//   }
//   else if (layerType == "linearnobias")
//   {
//     NoRegularizer regularizer;
//     origParams["insize"] = NAN;
//     origParams["outsize"] = NAN;
//     updateParams(origParams, layerParams);
//     layer = new LinearNoBias<>(origParams["insize"], origParams["outsize"],
//         regularizer);
//   }
  // if (layerType == "linear")
  // {
  //   NoRegularizer regularizer;
  //   origParams["insize"] = NAN;
  //   origParams["outsize"] = NAN;
  //   updateParams(origParams, layerParams);
  //   layer = new Linear<>(origParams["insize"], origParams["outsize"],
  //       regularizer);
  // }
//   else if (layerType == "maxpooling")
//   {
//     origParams["kw"] = NAN;
//     origParams["kh"] = NAN;
//     origParams["dw"] = 1;
//     origParams["dh"] = 1;
//     origParams["floor"] = 1; // 1 for true, 0 for false
//     updateParams(origParams, layerParams);
//     layer = new MaxPooling<>(origParams["kw"], origParams["kh"],
//         origParams["dw"], origParams["dh"], origParams["floor"]);
//   }
//   else if (layerType == "meanpooling")
//   {
//     origParams["kw"] = NAN;
//     origParams["kh"] = NAN;
//     origParams["dw"] = 1;
//     origParams["dh"] = 1;
//     origParams["floor"] = true;
//     updateParams(origParams, layerParams);
//     layer = new MeanPooling<>(origParams["kw"], origParams["kh"],
//         origParams["dw"], origParams["dh"], origParams["floor"]);
//   }
//   else if (layerType == "multiplyconstant")
//   {
//     origParams["scalar"] = 1.0;
//     updateParams(origParams, layerParams);
//     layer = new MultiplyConstant<>(origParams["scalar"]);
//   }
//   else if (layerType == "transposedconvolution")
//   {
//     origParams["insize"] = NAN;
//     origParams["outsize"] = NAN;
//     origParams["kw"] = NAN;
//     origParams["kh"] = NAN;
//     origParams["dw"] = 1;
//     origParams["dh"] = 1;
//     origParams["padw"] = 0;
//     origParams["padh"] = 0;
//     origParams["inputwidth"] = 0;
//     origParams["inputheight"] = 0;
//     origParams["outputwidth"] = 0;
//     origParams["outputheight"] = 0;
//     origParams["paddingtype"] = 0; // None = 0 , Valid = 1, Same = 2
//     updateParams(origParams, layerParams);
//     std::string padding = decodePadType(origParams["paddingtype"]);
//     layer = new TransposedConvolution<>(origParams["insize"],
//         origParams["outsize"], origParams["kw"], origParams["kh"],
//         origParams["dw"], origParams["dh"], origParams["padw"],
//         origParams["padh"], origParams["inputwidth"],
//         origParams["inputheight"], origParams["outputwidth"],
//         origParams["outputheight"], padding);
//   }
  // if (layerType == "identity")
  // {
  //   layer = new IdentityLayer<>();
  // }
//   else if (layerType == "rectifier" || layerType == "relu")
//   {
//     layer = new ReLULayer<>();
//   }
//   else if (layerType == "tanh")
//   {
//     layer = new TanHLayer<>();
//   }
//   else if (layerType == "elu")
//   {
//     origParams["alpha"] = NAN;
//     updateParams(origParams, layerParams);
//     layer = new ELU<>(origParams["alpha"]);
//   }
//   else if (layerType == "selu")
//   {
//     layer = new SELU;
//   }
//   else if (layerType == "hardtanh")
//   {
//     origParams["maxvalue"] = 1;
//     origParams["minvalue"] = -1;
//     updateParams(origParams, layerParams);
//     layer = new HardTanH<>(origParams["maxvalue"], origParams["minvalue"]);
//   }
//   else if (layerType == "leakyrelu")
//   {
//     origParams["alpha"] = 0.03;
//     updateParams(origParams, layerParams);
//     layer = new LeakyReLU<>(origParams["alpha"]);
//   }
//   else if (layerType == "prelu")
//   {
//     origParams["alpha"] = 0.03; // userAlpha
//     layer = new PReLU<>(origParams["alpha"]);
//   }
//   else if (layerType == "sigmoid")
//   {
//     layer = new SigmoidLayer<>();
//   }
//   else if (layerType == "softmax")
//   {
//     layer = new Softmax<>();
//   }
  if (layerType == "logsoftmax")
{
  // LogSoftMax logSoftMaxLayer = new LogSoftMax();
  // layer = &logSoftMaxLayer; // Now you're assigning a valid Layer pointer
    layer = new LogSoftMax();

}

//   else
//   {
//     Log::Fatal << "Invalid layer type : " << layerType;
//   }

  return layer;
}











/**
 * Get the mlpack layer associated with the given layer type
 * instantiated with the given parameters
 *
 * @param node The ONNX node containing the layer attributes
 * @param dimParams The map containing information about the
 * dimensions of the layer
 */
Layer<> *getLayer(const NodeProto& node, string layerType,
                    map<string, double>& dimParams)
{
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





  map<string, vector<string>> layer = mappedNames[layerType];
  /*stores the attributes which can be calculated
  only after knowing the other ones*/
  vector<string> skippedAttributes;

  // node {
  //   input: "ReLU402_Output_0"
  //   output: "Pooling404_Output_0"
  //   name: "Pooling66"
  //   op_type: "MaxPool"
  //   attribute {
  //     name: "kernel_shape"
  //     ints: 2
  //     ints: 2
  //     type: INTS
  //   }
  //   attribute {
  //     name: "strides"
  //     ints: 2
  //     ints: 2
  //     type: INTS
  //   }
  //   attribute {
  //     name: "pads"
  //     ints: 0
  //     ints: 0
  //     ints: 0
  //     ints: 0
  //     type: INTS
  //   }
  //   doc_string: ""
  //   domain: ""
  // }
  
  for (AttributeProto attribute:node.attribute())
  {
    string attrName = attribute.name();
    vector<string> attr = layer[attrName];
    vector<string>::iterator itr;

    //check for special cases
    if (attrName == "pads")
    {
      // [0 1 2 3] indices are top, bottom, left, right respectively
      mappedParams["padw"] = (int) (attribute.ints()[1] + attribute.ints()[3]) / 2;
      mappedParams["padh"] = (int) (attribute.ints()[0] + attribute.ints()[2]) / 2;
      continue;
    }
    else if (attrName == "auto_pad")
    {
      // P = ((S-1)*W-S+F)/2
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
  return getNetworkReference(operatorMap[layerType], mappedParams);
}










/**
 * Get the mlpack equivalent model of a given ONNX model without
 * the transfer of weights
 *
 * @param graph The ONNX graph containing all the layer details
 * @return An mlpack FFN model corresponding to the ONNX model passed
 */
FFN<> generateModel(GraphProto& graph)
{
  FFN<> mod;
  std::vector<int> dims = findWeightDims(graph.initializer());
  std::vector<int>::iterator itr = dims.begin();
  for (auto nodeItr = std::begin(graph.node()); nodeItr != std::end(graph.node()); ++nodeItr)
  {
    string nodeType = nodeItr -> op_type();
    map<string, double> dimParams;
    std::vector<string> skipLayers = {"Add", "Identity", "Reshape", "Transpose", "Unsqueeze",
    "Shape", "Cast", "Slice", "Concat", "ReduceProd"};
    std::vector<string> dimensionalLayers = {"ConstantOfShape", "Conv", "Gemm", "Matmul",
    "ConvTranspose", "Transformed_linear"};// may be an error with the ConstantOfShape layer
    // layers for merging, currently just one is there
    std::map<std::vector<string>, string> mergeLayers = {
        {{"MatMul", "Add"}, "Transformed_linear"}
        };
    std::map<std::vector<string>, string>::iterator mergeItr;
    for (mergeItr = mergeLayers.begin(); mergeItr != mergeLayers.end(); ++mergeItr)
    {
      std::vector<string> mergeVector = mergeItr -> first;
      int i;
      for (i = 0; i < mergeVector.size(); ++i)
      {
        //cout << "i= " << i << " mervector[i]= " << mergeVector[i] << " op_type= " << (nodeItr + i) -> op_type() << "\n";
        if ((nodeItr + i) == std::end(graph.node()) || mergeVector[i] != (nodeItr + i) -> op_type())
          break;
      }
      //cout << "i= " << i << " mergeVector.size()= " << mergeVector.size() << "\n";
      if (i == mergeVector.size())
      {
        nodeItr += i;
        nodeType = mergeItr -> second;
      }
    }

    if (std::find(skipLayers.begin(), skipLayers.end(), nodeType) != skipLayers.end())
      continue;

    if (std::find(dimensionalLayers.begin(), dimensionalLayers.end(), nodeType) != dimensionalLayers.end())
    {
      dimParams["insize"] = *itr;
      itr++;
      dimParams["outsize"] = *itr;
    }
    if(nodeType == "Conv")
    {
      dimParams["inputwidth"] = storedParams["inputwidth"];
      dimParams["inputheight"] = storedParams["inputheight"];
    }
    std::cout << "Node type: " << nodeType << "\n";
    printMap(dimParams);
    // BaseLayer<> b = getLayer(*nodeItr, nodeType, dimParams);
    mod.Add(getLayer(*nodeItr, nodeType, dimParams));
  }
  return mod;
}








int main()
{
    cout<<"the code is working fine"<<endl;
    return 0;
}