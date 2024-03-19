
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
BaseLayer<> getNetworkReference(std::string& layerType,
                                 std::map<std::string, double>& layerParams)
{

  std::map<std::string, double> origParams;
  BaseLayer<> layer;

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
  if (layerType == "convolution")
  {
    origParams["insize"] = NAN;
    origParams["outsize"] = NAN;
    origParams["kw"] = NAN;
    origParams["kh"] = NAN;
    origParams["dw"] = 1;
    origParams["dh"] = 1;
    origParams["padw"] = 0;
    origParams["padh"] = 0;
    origParams["inputwidth"] = 0;
    origParams["inputheight"] = 0;
    origParams["paddingtype"] = 0;
    updateParams(origParams, layerParams);
    std::string padding = decodePadType(origParams["paddingtype"]);
    layer = new Convolution<>(origParams["insize"], origParams["outsize"],
        origParams["kw"], origParams["kh"], origParams["dw"],
        origParams["dh"], origParams["padw"], origParams["padh"],
        origParams["inputwidth"], origParams["inputheight"], padding);
  }
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
//   else if (layerType == "linear")
//   {
//     NoRegularizer regularizer;
//     origParams["insize"] = NAN;
//     origParams["outsize"] = NAN;
//     updateParams(origParams, layerParams);
//     layer = new Linear<>(origParams["insize"], origParams["outsize"],
//         regularizer);
//   }
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
//   else if (layerType == "logsoftmax")
//   {
//     layer = new LogSoftMax<>();
//   }
//   else
//   {
//     Log::Fatal << "Invalid layer type : " << layerType;
//   }

  return layer;
}




int main()
{
    cout<<"the program is working fine"<<endl;
    return 0;
}