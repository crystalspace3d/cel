/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Jorrit Tyberghein

    Neural Network Property Class
    Copyright (C) 2007 by Mat Sutcliffe

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_NEURALNET__
#define __CEL_PF_NEURALNET__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/refcount.h"
#include "csutil/array.h"

#include "physicallayer/datatype.h"

#include <math.h>
#include <float.h>

class celNNActivationFunc;

/**
 * Structure which can hold the weightings of all links between the nodes of
 * an iPcNeuralNet property class. This is not really intended to be used by
 * user code, only by neural network training code like celPcEvolve.
 *
 * Data()[i][j][k] is the weighting from node k to node j at layer i.
 *
 * \warning You must only modify the contents of the innermost arrays.
 *   Meaning, you must not replace any outer array, for example
 *   <tt>Data()[i] = myArray</tt> to replace an entire layer. If you do,
 *   you will cause memory allocation problems. You must instead copy each
 *   individual float value separately.
 */
struct iCelNNWeights : public virtual iBase
{ 
  SCF_INTERFACE(iCelNNWeights, 0, 0, 1);

  /// Access the weighting data (non-const). Use with care.
  virtual csArray< csArray< csArray<float> > >& Data() = 0;

  /// Access the weighting data.
  virtual const csArray< csArray< csArray<float> > >& Data() const = 0;

  /// Returns a new structure which is a copy of this one.
  virtual csPtr<iCelNNWeights> Clone() const = 0;
};

/**
 * A property class implementing a feed-forward neural network.
 *
 * The property class holds the following properties (add prefix
 * "cel.property." to get a property ID):
 * - "inputs" (long) The number of inputs in the neural network.
 * - "outputs" (long) The number of outputs in the neural network.
 * - "layers" (long) The number of hidden layers in the neural network.
 * - "dispatch" (bool) If true, pcneuralnet_outputs messages will be sent.
 *
 * And the following actions (add prefix "cel.action." or "cel.parameter."
 * to get the ID of an action or parameter respectively):
 * - "SetComplexity" Sets the method to use to set the sizes of the layers.
 *   Add prefix "cel.complexity." to the heuristic name to get the param.
 * - "SetLayerSizes" Instead of SetComplexity, this sets the sizes of the
 *   layers manually. Parameters: "layer0"..."layerN" (longs).
 * - "SetActivationFunc" Sets the activation function in the neural network.
 *   Parameter: "func" (string). See iCelNNActivationFunc and its subclasses.
 *   This also sets the datatype of the inputs and outputs.
 * - "SetInputs" Sets the values of the inputs. Parameters:
 *   "input0"..."inputN" the values (N = "cel.property.inputs" - 1).
 * - "Process" Runs one iteration through the neural network, reading the
 *   inputs and setting the outputs. May send "pcneuralnet_outputs" message.
 * - "SaveCache" Saves the weightings to VFS path /cellib/cache/pcneuralnet.
 *   Parameters: "scope" (string), "id" (long). See iCacheManager in CS.
 * - "LoadCache" Loads the weightings from VFS path /cellib/cache/pcneuralnet.
 *   Parameters: "scope" (string), "id" (long). See iCacheManager in CS.
 *
 * And can send the following message to the behaviour:
 * - "pcneuralnet_outputs" Sent after every call to "cel.action.process".
 *   Parameters: "output0"..."outputN" the output values from the neural
 *   network (N - "cel.property.outputs" - 1).
 */
struct iPcNeuralNet : public virtual iBase
{
  SCF_INTERFACE(iPcNeuralNet, 0, 0, 1);

  /**
   * Sets the size of the neural network (number of inputs, outputs and
   * hidden layers).
   */
  virtual void SetSize(size_t inputs, size_t outputs, size_t layers) = 0;

  /**
   * Sets the heuristic that will be used to set the sizes of each layer.
   *
   * Alternatively you can call SetLayerSizes to set the sizes manually.
   *
   * Acceptable values for the name parameter are:
   * - "linear" (default) sizes progress linearly from inputs to outputs.
   * - "halfLinear" like linear but first layer is 50% of the number of inputs.
   * - "addHalfLinear" line linear but first layer is 150% the number of inputs.
   */
  virtual void SetComplexity(const char *name) = 0;

  /**
   * Manually set the number of nodes in each hidden layer.
   *
   * Alternatively you can call SetComplexity to have the sizes set
   * automatically according to the selected heuristic.
   */
  virtual void SetLayerSizes(const csArray<size_t> &sizes) = 0;

  /**
   * Sets the activation function of the neural network.
   */
  virtual void SetActivationFunc(celNNActivationFunc *) = 0;

  /**
   * Returns true if the neural network is initialized and ready to use.
   * If it is not, then an attempt is made to initialize it, and the return
   * value signifies success or failure of that attempt.
   */
  virtual bool Validate() = 0;

  /**
   * Sets the value of one of the inputs.
   */
  virtual void SetInput(size_t index, const celData &value) = 0;

  /**
   * Returns the value of one of the outputs.
   */
  virtual const celData& GetOutput(size_t index) const = 0;

  /**
   * Sets all the input values in one go.
   */
  virtual void SetInputs(const csArray<celData> &values) = 0;

  /**
   * Retrieves all the output values in one go.
   */
  virtual const csArray<celData>& GetOutputs() const = 0;

  /**
   * Runs one iteration through the network, reading the inputs and setting
   * the outputs.
   */
  virtual void Process() = 0;

  /**
   * Returns a new weights structure with the same size as the current one.
   */
  virtual csPtr<iCelNNWeights> CreateEmptyWeights() const = 0;

  /**
   * Copies the weightings of the neural network into the structure provided.
   */
  virtual void GetWeights(iCelNNWeights *out) const = 0;

  /**
   * Copies the weightings into the neural network from the given structure.
   */
  virtual bool SetWeights(const iCelNNWeights *in) = 0;

  /**
   * Saves the weightings to cache with an iCacheManager.
   */
  virtual bool CacheWeights(const char *scope, uint32 id) const = 0;

  /**
   * Loads the weightings from cache with an iCacheManager.
   */
  virtual bool LoadCachedWeights(const char *scope, uint32 id) = 0;
};

/**
 * Abstract class representing an activation function callback for the
 * neural network property class.
 *
 * This is passed to the iPcNeuralNet::SetActivationFunc() method, and is
 * used as the activation function for the nodes in the network. The user of
 * the property class can use one of the supplied implementations of
 * celActivationFunc, or they can implement their own.
 *
 * The supplied implementations can also be selected using pcneuralnet's
 * "cel.action.SetActivationFunc" action. See the specific implementation
 * class for the "cel.activationFunc." string ID to use.
 */
class celNNActivationFunc : public virtual csRefCount
{
public:
  /// The callback method. Performs the activation function on the data.
  virtual void Function(celData &data) = 0;

  /// Returns the type of data upon which this function operates.
  virtual celDataType GetDataType() = 0;

  /// Destructor.
  virtual ~celNNActivationFunc() {}

protected:
  /**
   * Convenience template for subclasses of celNNActivationFunc.
   *
   * Implementations of celNNActivationFunc may, if they wish, use this method
   * template to extract data from a celData structure in a generic way,
   * since celData does not provide its own generic Get() method.
   *
   * The template is specialised for all integer types and float (not double).
   * Attempting to use any other type for the template parameter will result
   * in a linker error.
   */
  template <typename T>
  static const T& GetFrom(const celData &input);

  /**
   * Convenience template for subclasses of celNNActivationFunc.
   *
   * Implementations of celNNActivationFunc may, if they wish, use this method
   * template to translate a C++ type declarator into a CEL_DATA constant.
   *
   * The template is specialised for all integer types and float (not double).
   * Attempting to use any other type for the template parameter will result
   * in a linker error.
   */
  template <typename T>
  static celDataType DataType();
};

template<>
inline const float& celNNActivationFunc::GetFrom<float>(const celData &input)
{
  return input.value.f;
}
template<>
inline const int8& celNNActivationFunc::GetFrom<int8>(const celData &input)
{
  return input.value.b;
}
template<>
inline const uint8& celNNActivationFunc::GetFrom<uint8>(const celData &input)
{
  return input.value.ub;
}
template<>
inline const int16& celNNActivationFunc::GetFrom<int16>(const celData &input)
{
  return input.value.w;
}
template<>
inline const uint16& celNNActivationFunc::GetFrom<uint16>(const celData &input)
{
  return input.value.uw;
}
template<>
inline const int32& celNNActivationFunc::GetFrom<int32>(const celData &input)
{
  return input.value.l;
}
template<>
inline const uint32& celNNActivationFunc::GetFrom<uint32>(const celData &input)
{
  return input.value.ul;
}

template<>
inline celDataType celNNActivationFunc::DataType<int8>()
{
  return CEL_DATA_BYTE;
}
template<>
inline celDataType celNNActivationFunc::DataType<uint8>()
{
  return CEL_DATA_UBYTE;
}
template<>
inline celDataType celNNActivationFunc::DataType<int16>()
{
  return CEL_DATA_WORD;
}
template<>
inline celDataType celNNActivationFunc::DataType<uint16>()
{
  return CEL_DATA_UWORD;
}
template<>
inline celDataType celNNActivationFunc::DataType<int32>()
{
  return CEL_DATA_LONG;
}
template<>
inline celDataType celNNActivationFunc::DataType<uint32>()
{
  return CEL_DATA_ULONG;
}
template<>
inline celDataType celNNActivationFunc::DataType<float>()
{
  return CEL_DATA_FLOAT;
}

/**
 * An identity activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.none", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celNopActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data) {}
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celNopActivationFunc() {}
};

/**
 * A threshold activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.step", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celStepActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set(T (val > 1 ? 1 : 0));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celStepActivationFunc() {}
};

/**
 * A logarithmic activation function for the neural network property class.
 *
 * This template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.log", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celLogActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    double e_v = log(fabs((double) val)); // log may return not-a-number
    data.Set((T) (std::isnormal(e_v) ? e_v : 0.0));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celLogActivationFunc() {}
};

/**
 * An arc-tangent activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.atan", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celAtanActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set((T) atan((double) val));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celAtanActivationFunc() {}
};

/**
 * A hyperbolic-tangent activation function for the neural network property
 * class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.tanh", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celTanhActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set((T) tanh((double) val));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celTanhActivationFunc() {}
};

/**
 * An exponential activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.exp", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celExpActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    double e_v = exp((double) val); // exp may return infinite
    data.Set((T) (std::isnormal(e_v) ? e_v : 0.0));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celExpActivationFunc() {}
};

/**
 * A reciprocal activation function for the neural network property class.
 *
 * This activation function works on floats only.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.float.inv".
 */
class celInvActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const float &val = GetFrom<float>(data);
    data.Set(1.0f / val);
  }
  virtual celDataType GetDataType() { return CEL_DATA_FLOAT; }
  virtual ~celInvActivationFunc() {}
};

/**
 * A squaring activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.sqr", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celSqrActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set(val * val);
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celSqrActivationFunc() {}
};

/**
 * An gaussian activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.gauss", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celGaussActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set((T) exp((double) -(val * val)));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celGaussActivationFunc() {}
};

/**
 * A sine activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.sin", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celSinActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set((T) sin((double) val));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celSinActivationFunc() {}
};

/**
 * A cosine activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.cos", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celCosActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set((T) cos((double) val));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celCosActivationFunc() {}
};

/**
 * An Elliott activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.elliott", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celElliottActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set(val / (1 + (T)fabs((double) val)));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celElliottActivationFunc() {}
};

/**
 * A sigmoid activation function for the neural network property class.
 *
 * The template is valid for T = float and T = any integer type. Attempting to
 * use any other type for the template parameter will result in an error.
 *
 * This activation function can also be selected with pcneuralnet's
 * "cel.action.SetActivationFunc" action, with the string parameter
 * "cel.parameter.func" set to "cel.activationFunc.TYPE.sig", where TYPE is
 * either int8, int16, int32, uint8, uint16, uint32 or float.
 */
template <typename T>
class celSigActivationFunc : public celNNActivationFunc
{
public:
  virtual void Function(celData &data)
  {
    const T &val = GetFrom<T>(data);
    data.Set(T(1) / (T)(1 + exp((double) -val)));
  }
  virtual celDataType GetDataType() { return DataType<T>(); }
  virtual ~celSigActivationFunc() {}
};

#endif // __CEL_PF_NEURALNET__

