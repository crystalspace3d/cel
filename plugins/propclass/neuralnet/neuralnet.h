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

#ifndef __CEL_PF_NEURALNETFACT__
#define __CEL_PF_NEURALNETFACT__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/csstring.h"
#include "iutil/comp.h"
#include "iutil/cache.h"

#include "propclass/neuralnet.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"

struct iObjectRegistry;
struct iDataBuffer;

CEL_DECLARE_FACTORY(NeuralNet)

/**
 * The neural network implementation, incorporating its CEL property class.
 */
class celPcNeuralNet : public
  scfImplementationExt1<celPcNeuralNet, celPcCommon, iPcNeuralNet>
{
  static PropertyHolder propinfo;
  csRef<celGenericParameterBlock> params;

  csRef<iMessageDispatcher> dispatcher_outputs;

  /// Pointer to a function which returns a new celNNActivationFunc.
  typedef celNNActivationFunc* (*funcgen_t)();

  /// Hash associating csStringIDs with celNNActivationFunc generators.
  static csHash<funcgen_t, csStringID> funcgens;

  /// The number of inputs, outputs and hidden layers.
  int32 numInputs, numOutputs, numLayers;

  /// Boolean indicating whether to send pcneuralnet_outputs messages.
  bool doDispatch;

  /// String specifying a heuristic to use to set the sizes of the layers.
  csString sizeHeuristic;

  /// The layer sizes, set manually or automatically.
  csArray<size_t> layerSizes;

  /// The activation function for the neural network.
  csRef<celNNActivationFunc> actFunc;

  /// Pointer to a function which performs a weighted sum for each node.
  typedef void (* datahandler_t)(celData&, csArray<celData>&, csArray<float>&);

  /// The datahandler_t function specialised to a specific datatype.
  datahandler_t datahandler;

  /// A celData structure holding a zero value of a specific datatype.
  celData zeroData;

  /// The object to use to load/save the weights in cache.
  csRef<iCacheManager> cache;

  /**
   * This class holds all the data pertaining to one hidden layer.
   */
  struct HiddenLayer
  {
    csArray< csArray<float> > weights;

    void Initialize(size_t nodes, size_t prenodes)
    {
      csArray<float> innerCopy;
      innerCopy.SetSize(prenodes, 1.0 / (float)prenodes);
      weights.DeleteAll();
      weights.SetSize(nodes, innerCopy);
    }

    void Process(csArray<celData> &in, csArray<celData> &out,
	celNNActivationFunc*, datahandler_t);
  };

  /// The inputs, written to by the game logic and read by the uppermost hidden layer.
  csArray<celData> inputs;

  /// The outputs, written to by the bottom hidden layer and read by the game logic.
  csArray<celData> outputs;

  /// The array of hidden layer structures.
  csArray<HiddenLayer> layers;

  /// Boolean indicating whether the neural network is initialised and ready to use.
  bool valid;

  /**
   * Initialize the hidden layer structures to have the correct sizes.
   */
  bool InitLayerSizes();

  /**
   * Utility function called by InitLayerSizes(). Sets the sizes of the layers
   * in a linear progression from size1 to numOutputs.
   */
  void LinearLayerSizes(int size1);

  /**
   * The private method called by the "cel.action.SetComplexity" action.
   * Sets the heuristic that will be used to set the sizes of the layers.
   */
  bool SetComplexity(iCelParameterBlock *params);

  /**
   * The private method called by the "cel.action.SetLayerSizes" action.
   * Manually sets the sizes of the hidden layers.
   */
  bool SetLayerSizes(iCelParameterBlock *params);

  /**
   * The private method called by the "cel.action.SetActivationFunc" action.
   * Translates a descriptive string into a celActivationFunc.
   */
  bool SetActivationFunc(iCelParameterBlock *params);

  /**
   * The private method called by the "cel.action.SetInputs" action. Sets all
   * the inputs in one go.
   */
  bool SetInputs(iCelParameterBlock *params);

  /**
   * The private method called by the "cel.action.Process" action. Runs one
   * iteration through the neural network, reading inputs and setting outputs.
   */
  bool ProcessLayers();

  /**
   * Sends the pcneuralnet_outputs message to the behaviour.
   */
  void SendMessage();

  /**
   * The private method called by the "cel.action.SaveCache" action. Saves
   * the weightings to the iCacheManager.
   */
  bool SaveCache(iCelParameterBlock *params);

  /**
   * The private method called by the "cel.action.LoadCache" action. Loads
   * the weightings from the iCacheManager.
   */
  bool LoadCache(iCelParameterBlock *params);

  /**
   * Convenience method used by LoadCachedGenome(). Reads an int from a buffer.
   * Converts endianness. Does a bounds check first and reports error if failed.
   */
  int32 ReadInt32(iDataBuffer *buf, size_t index);

public:
  /// Constructor.
  celPcNeuralNet(iObjectRegistry *objreg);

  /// Destructor.
  virtual ~celPcNeuralNet();

  /// Report error. Always returns false.
  bool Error(char const *desc, ...) const;
  /// Report warning. Always returns false.
  bool Warning(char const *desc, ...) const;
  /// Report bug. Always returns false.
  bool Bug(char const *desc, ...) const;

  //--------------------------------------------------------- celPcCommon --//
  virtual bool PerformActionIndexed(int index, iCelParameterBlock *params, celData &ret);
  virtual bool Load(iCelDataBuffer *) { return true; }
  virtual csPtr<iCelDataBuffer> Save() { return pl->CreateDataBuffer(0); }

  //-------------------------------------------------------- iPcNeuralNet --//
  virtual void SetSize(size_t ins, size_t outs, size_t lays)
  { numInputs = ins; numOutputs = outs; numLayers = lays; }
  virtual void SetLayerSizes(const csArray<size_t> &sizes)
  { CS_ASSERT(sizes.GetSize() == size_t(numLayers));
    layerSizes = sizes; sizeHeuristic = "none"; }
  virtual void SetComplexity(const char *name) { sizeHeuristic = name; }
  virtual void SetActivationFunc(celNNActivationFunc *func) { actFunc = func; }
  virtual void SetInput(size_t i, const celData &value)
  { CS_ASSERT_MSG("Input index out of bounds", i < size_t(numInputs));
    inputs[i] = value; }
  virtual const celData& GetOutput(size_t i) const
  { CS_ASSERT_MSG("Output index out of bounds", i < size_t(numOutputs));
    return outputs[i]; }
  virtual void SetInputs(const csArray<celData> &vals)
  { CS_ASSERT_MSG("Array is wrong size", vals.GetSize() == size_t(numInputs));
    inputs = vals; }
  virtual const csArray<celData>& GetOutputs() const { return outputs; }

  virtual void Process() { ProcessLayers(); }
  virtual bool Validate();
  virtual void GetWeights(iCelNNWeights *out) const;
  virtual bool SetWeights(const iCelNNWeights *in);
  virtual bool CacheWeights(const char *scope, uint32 id) const;
  virtual bool LoadCachedWeights(const char *scope, uint32 id);

  class Weights : public scfImplementation1<Weights, iCelNNWeights>
  {
    csArray< csArray< csArray<float> > > data;

  public:
    Weights(const csArray<HiddenLayer> &tpl);
    Weights(const Weights &other)
    : scfImplementationType (this), data (other.data) {}
    virtual ~Weights() {}

    csArray< csArray< csArray<float> > >& Data() { return data; }
    const csArray< csArray< csArray<float> > >& Data() const { return data; }
    csPtr<iCelNNWeights> Clone() const { return new Weights (*this); }
  };

  virtual csPtr<iCelNNWeights> CreateEmptyWeights() const
  { return new Weights (layers); }
};

#endif // __CEL_PF_NEURALNETFACT__

