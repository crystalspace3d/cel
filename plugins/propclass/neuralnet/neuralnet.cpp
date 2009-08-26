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

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "csutil/databuf.h"
#include "csutil/vfscache.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/csendian.h"
#include "ivaria/reporter.h"

#include "neuralnet.h"

#include <math.h>

CS_IMPLEMENT_PLUGIN
CEL_IMPLEMENT_FACTORY(NeuralNet, "pcai.neuralnet")

// portable rounding function
int nnRound(double x)
{
  double integer;
  double fraction = modf(x, &integer);
  int i = (int) integer;
  if (x > 0.0) return (fraction <  0.5) ? i : i + 1;
	  else return (fraction > -0.5) ? i : i - 1;
}

template <typename T>   T& accessData	     (celData &);
template <> inline   int8& accessData<int8>  (celData &d) { return d.value.b; }
template <> inline  int16& accessData<int16> (celData &d) { return d.value.w; }
template <> inline  int32& accessData<int32> (celData &d) { return d.value.l; }
template <> inline  uint8& accessData<uint8> (celData &d) { return d.value.ub; }
template <> inline uint16& accessData<uint16>(celData &d) { return d.value.uw; }
template <> inline uint32& accessData<uint32>(celData &d) { return d.value.ul; }
template <> inline  float& accessData<float> (celData &d) { return d.value.f; }

// Generic weighted sum
template <typename T>
void handler(celData &out, csArray<celData> &in, csArray<float> &weights)
{
  CS_ASSERT(in.GetSize() == weights.GetSize());

  for (size_t i = 0, end = in.GetSize(); i < end; i++)
  {
    accessData<T>(out) += (T)(accessData<T>(in[i]) * weights[i]);
  }
}

enum // actionID
{
  NN_SETFUNC = 0,
  NN_SETCOMP,
  NN_SETLAYERS,
  NN_SETINPUTS,
  NN_PROCESS,
  NN_SAVECACHE,
  NN_LOADCACHE
};

enum // propertyID
{
  NN_INPUTS = 0,
  NN_OUTPUTS,
  NN_LAYERS,
  NN_DISPATCH,
  NN_PROP_LAST
};

const char* const descriptions[] = {
  "The number of inputs of the neural network.",
  "The number of outputs of the neural network.",
  "The number of hidden layers of the neural network.",
  "Boolean indicating whether to send pcneuralnet_outputs messages."
};

// Instances of these templates are associated with csStringIDs in a csHash,
// so that the activation function can be selected with a string param passed
// to "SetActivationFunc".
template <typename T>
celNNActivationFunc* gen_nop() { return new celNopActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_step() { return new celStepActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_log() { return new celLogActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_atan() { return new celAtanActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_tanh() { return new celTanhActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_exp() { return new celExpActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_sqr() { return new celSqrActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_gauss() { return new celGaussActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_sin() { return new celSinActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_cos() { return new celSinActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_elliott() { return new celElliottActivationFunc<T>; }
template <typename T>
celNNActivationFunc* gen_sig() { return new celSigActivationFunc<T>; }
celNNActivationFunc* gen_inv() { return new celInvActivationFunc; }

csHash<celPcNeuralNet::funcgen_t, csStringID> celPcNeuralNet::funcgens;

PropertyHolder celPcNeuralNet::propinfo;

celPcNeuralNet::celPcNeuralNet(iObjectRegistry *objreg)
: scfImplementationType (this, objreg),
  numInputs (0),
  numOutputs (0),
  numLayers (0),
  doDispatch (false),
  sizeHeuristic ("linear"),
  valid (false)
{
  cache.AttachNew(new csVfsCacheManager (objreg, "/cellib/cache"));

  SetName("pcneuralnet");
  propholder = &propinfo;

  if (! propinfo.actions_done)
  {
    SetActionMask ("cel.neural.action.");
    AddAction(NN_SETFUNC, "SetActivationFunc");
    AddAction(NN_SETCOMP, "SetComplexity");
    AddAction(NN_SETLAYERS, "SetLayerSizes");
    AddAction(NN_SETINPUTS, "SetInputs");
    AddAction(NN_PROCESS, "Process");
    AddAction(NN_SAVECACHE, "SaveCache");
    AddAction(NN_LOADCACHE, "LoadCache");
  }

  propinfo.SetCount(NN_PROP_LAST);
  AddProperty(NN_INPUTS, "inputs", CEL_DATA_LONG, false,
	descriptions[NN_INPUTS], &numInputs);
  AddProperty(NN_OUTPUTS, "outputs", CEL_DATA_LONG, false,
	descriptions[NN_OUTPUTS], &numOutputs);
  AddProperty(NN_LAYERS, "layers", CEL_DATA_LONG, false,
	descriptions[NN_LAYERS], &numLayers);
  AddProperty(NN_DISPATCH, "dispatch", CEL_DATA_BOOL, false,
	descriptions[NN_DISPATCH], &doDispatch);

  // Associate funcgen_t function pointers with csStringIDs.
  if (funcgens.GetSize() == 0)
  {
    #define T_GENREG(T, F) funcgens.Put \
      (pl->FetchStringID("cel.activationFunc." #T "." #F), gen_##F <T>);

    #define GENREG(F) T_GENREG(float, F) \
      T_GENREG(int8, F) T_GENREG(int16, F) T_GENREG(int32, F) \
      T_GENREG(uint8, F) T_GENREG(uint16, F) T_GENREG(uint32, F)

    GENREG(nop)
    GENREG(step)
    GENREG(log)
    GENREG(atan)
    GENREG(tanh)
    GENREG(exp)
    GENREG(sqr)
    GENREG(gauss)
    GENREG(sin)
    GENREG(cos)
    GENREG(elliott)
    GENREG(sig)
    funcgens.Put(pl->FetchStringID("cel.activationFunc.float.inv"), gen_inv);

    #undef GENREG
    #undef T_GENREG
  }
}

celPcNeuralNet::~celPcNeuralNet()
{
}

bool celPcNeuralNet::Validate()
{
  if (valid) return true;

  if (numInputs <= 0 || numOutputs <= 0 || numLayers <= 0)
    return Error("One or more properties have invalid values.");
  if (! actFunc.IsValid())
    return Error("No activation function selected.");

  // Initialize parts which are specific to the datatype being used.
  switch (actFunc->GetDataType())
  {
    case CEL_DATA_BYTE:
	zeroData.Set(int8(0));	 datahandler = handler<int8>;	break;
    case CEL_DATA_WORD:
	zeroData.Set(int16(0));	 datahandler = handler<int32>;	break;
    case CEL_DATA_LONG:
	zeroData.Set(int32(0));	 datahandler = handler<int16>;	break;
    case CEL_DATA_UBYTE:
	zeroData.Set(uint8(0));	 datahandler = handler<uint8>;	break;
    case CEL_DATA_UWORD:
	zeroData.Set(uint16(0)); datahandler = handler<uint16>;	break;
    case CEL_DATA_ULONG:
	zeroData.Set(uint32(0)); datahandler = handler<uint32>;	break;
    case CEL_DATA_FLOAT:
	zeroData.Set(0.0f);	 datahandler = handler<float>;	break;
    default:
	return Error("Unsupported datatype for activation function.");
  }

  inputs.SetSize(size_t(numInputs));
  outputs.SetSize(size_t(numOutputs));
  layers.SetSize(size_t(numLayers + 1));
  if (! InitLayerSizes()) return false;

  params.AttachNew(new celGenericParameterBlock (numOutputs));
  for (size_t i = 0; i < size_t(numOutputs); i++)
  {
    csString id ("output"); id << i;
    params->SetParameterDef(i, pl->FetchStringID(id));
  }

  valid = true;
  return true;
}

bool celPcNeuralNet::InitLayerSizes()
{
  if (sizeHeuristic == "none")
  {}
  else if (sizeHeuristic == "linear")
  {
    LinearLayerSizes(numInputs);
  }
  else if (sizeHeuristic == "halfLinear")
  {
    LinearLayerSizes(nnRound(numInputs * 0.5));
  }
  else if (sizeHeuristic == "addHalfLinear")
  {
    LinearLayerSizes(nnRound(numInputs * 1.5));
  }
  else
    return Error("Unsupported size heuristic '%s'", sizeHeuristic.GetData());

  layers[0].Initialize(layerSizes[0], size_t(numInputs));
  for (size_t i = 1; i < size_t(numLayers); i++)
  {
    layers[i].Initialize(layerSizes[i], layerSizes[i - 1]);
  }
  layers.Top().Initialize(size_t(numOutputs), layerSizes.Top());

  return true;
}

void celPcNeuralNet::LinearLayerSizes(int size1)
{
  layerSizes.DeleteAll();
  double interval = numOutputs - size1;
  double step = interval / numLayers;

  for (size_t i = 0; i < size_t(numLayers); i++)
  {
    layerSizes.Push(size_t(nnRound(size1 + step * i)));
  }
}

bool celPcNeuralNet::PerformActionIndexed(int index, iCelParameterBlock *params, celData &ret)
{
  switch (index)
  {
    case NN_SETFUNC: return SetActivationFunc(params);
    case NN_SETCOMP: return SetComplexity(params);
    case NN_SETLAYERS: return SetLayerSizes(params);
    case NN_SETINPUTS: return SetInputs(params);
    case NN_PROCESS: return ProcessLayers();
    case NN_SAVECACHE: return SaveCache(params);
    case NN_LOADCACHE: return LoadCache(params);
    default:;
  }
  return false;
}

bool celPcNeuralNet::SetActivationFunc(iCelParameterBlock *params)
{
  if (params->GetParameterCount() != 1
   || params->GetParameterByIndex(0)->type != CEL_DATA_STRING)
    return Error("SetActivationFunc takes a single string parameter.");

  iString *name = params->GetParameterByIndex(0)->value.s;
  csStringID id = pl->FetchStringID(name->GetData());

  // Retrieve the function pointer associated with the given csStringID,
  // and use it to instantiate the celNNActivationFunc implementation.
  funcgen_t gen = funcgens.Get(id, 0);
  if (gen == 0)
    return Error("No such activation function '%s'", name->GetData());
  actFunc.AttachNew(gen());
  return true;
}

bool celPcNeuralNet::SetComplexity(iCelParameterBlock *params)
{
  if (params->GetParameterCount() != 1
   || params->GetParameterByIndex(0)->type != CEL_DATA_STRING)
    return Error("SetComplexity takes a single string parameter.");

  const char *prefix = "cel.complexity.";
  size_t prelen = strlen(prefix);

  iString *name = params->GetParameterByIndex(0)->value.s;
  if (name->StartsWith(prefix)) // Store the suffix in sizeHeuristic.
    sizeHeuristic = name->Slice(prelen, name->Length() - prelen)->GetData();
  else
    return Error("No such complexity heuristic '%s'", name->GetData());

  return true;
}

bool celPcNeuralNet::SetLayerSizes(iCelParameterBlock *params)
{
  if (params->GetParameterCount() != size_t(numLayers))
    return Error("SetLayerSizes takes %d parameters.");

  layerSizes.DeleteAll();
  for (size_t i = 0; i < size_t(numLayers); i++)
  {
    const celData *data = params->GetParameterByIndex(i);
    if (data->type != CEL_DATA_LONG)
      return Error("SetLayerSizes parameters must be long integers.");
    layerSizes.Push(size_t(data->value.l));
  }
  sizeHeuristic = "none";
  return true;
}

bool celPcNeuralNet::SaveCache(iCelParameterBlock *params)
{
  const celData *scope_d = params->GetParameter
	(pl->FetchStringID("scope"));
  const celData *id_d = params->GetParameter
	(pl->FetchStringID("id"));
  if (! (scope_d && scope_d->type == CEL_DATA_STRING
	&& id_d && id_d->type == CEL_DATA_LONG))
    return Error("SaveCache takes 2 parameters, string 'scope' and long 'id'.");
  iString *scope = scope_d->value.s;
  uint32 id = uint32(id_d->value.l);

  return CacheWeights(scope->GetData(), id);
}

bool celPcNeuralNet::LoadCache(iCelParameterBlock *params)
{
  const celData *scope_d = params->GetParameter
	(pl->FetchStringID("scope"));
  const celData *id_d = params->GetParameter
	(pl->FetchStringID("id"));
  if (! (scope_d && scope_d->type == CEL_DATA_STRING
	&& id_d && id_d->type == CEL_DATA_LONG))
    return Error("LoadCache takes 2 parameters, string 'scope' and long 'id'.");
  iString *scope = scope_d->value.s;
  uint32 id = uint32(id_d->value.ul);

  return LoadCachedWeights(scope->GetData(), id);
}

bool celPcNeuralNet::SetInputs(iCelParameterBlock *params)
{
  if (! Validate()) return Error("SetInputs: propclass not properly set up.");
  if (params->GetParameterCount() != size_t(numInputs))
    return Error("SetInputs takes %d parameters.", numInputs);

  for (size_t i = 0; i < size_t(numInputs); i++)
    inputs[i] = * params->GetParameterByIndex(i);

  return true;
}

bool celPcNeuralNet::ProcessLayers()
{
  if (! Validate()) return Error("Process: propclass not properly set up.");

#ifdef CS_DEBUG
  for (size_t i = 0; i < size_t(numInputs); i++)
  {
    if (inputs[i].type != actFunc->GetDataType())
      return Error("Type mismatch for input%u.", i);
  }
#endif

  csArray<celData> temp, temp2;
  temp = inputs; // initially, temp contains the input values
  for (size_t i = 0; i < size_t(numLayers + 1); i++)
  {
    temp2.Empty();
    temp2.SetSize(layers[i].weights.GetSize(), zeroData);

    // process layer i, with input values in temp, store output values in temp2
    layers[i].Process(temp, temp2, actFunc, datahandler);

    temp = temp2; // outputs from layer i become the inputs to layer i+1
  }
  outputs = temp; // copy the final outputs into the outputs array

  if (doDispatch) SendMessage();

  return true;
}

void celPcNeuralNet::HiddenLayer::Process(csArray<celData> &in,
  csArray<celData> &out, celNNActivationFunc *func, datahandler_t handler)
{
  for (size_t i = 0, end = out.GetSize(); i < end; i++)
  {
    celData &out_i = out[i];
    handler(out_i, in, weights[i]); // weighted sum
    func->Function(out_i); // activation function
  }
}

void celPcNeuralNet::SendMessage()
{
  for (size_t i = 0; i < size_t(numOutputs); i++)
    params->GetParameter(i) = outputs[i];
  celData ret;
  if (entity->GetBehaviour ())
    entity->GetBehaviour()->SendMessage("pcneuralnet_outputs",
	this, ret, params);
  if (!dispatcher_outputs)
  {
    dispatcher_outputs = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, "cel.neuralnet.outputs");
    if (!dispatcher_outputs) return;
  }
  dispatcher_outputs->SendMessage (params);
}

celPcNeuralNet::Weights::Weights(const csArray<HiddenLayer> &tpl)
  : scfImplementationType (this)
{
  data.SetSize(tpl.GetSize());

  for (size_t layer = 0, end = tpl.GetSize(); layer < end; layer++)
  {
    const csArray< csArray<float> > &weights = tpl[layer].weights;

    data[layer].SetSize(weights.GetSize());

    for (size_t i = 0, endi = weights.GetSize(); i < endi; i++)
    {
      data[layer][i].SetSize(weights[i].GetSize());
    }
  }
}

void celPcNeuralNet::GetWeights(iCelNNWeights *out) const
{
  out->Data().Empty();

  for (size_t i = 0, end = layers.GetSize(); i < end; i++)
  {
    out->Data().Push(layers[i].weights);
  }
}

bool celPcNeuralNet::SetWeights(const iCelNNWeights *in)
{
  if (layers.GetSize() != in->Data().GetSize())
    return Bug("SetWeights: Incompatible weights structure.");

  for (size_t i = 0, end = layers.GetSize(); i < end; i++)
  {
    if (layers[i].weights.GetSize() != in->Data()[i].GetSize())
      return Bug("SetWeights: Incompatible weights structure.");

    layers[i].weights = in->Data()[i];
  }
  return true;
}

bool celPcNeuralNet::CacheWeights(const char *scope, uint32 id) const
{
  if (! valid) return Error("SaveCache: propclass not properly set up.");
  if (! cache.IsValid()) return Error("No iCacheManager.");

  csDirtyAccessArray<int32> data;
  data.Push(csBigEndian::Int32(numInputs));
  data.Push(csBigEndian::Int32(numOutputs));
  data.Push(csBigEndian::Int32(numLayers));
  for (size_t layer = 0; layer < size_t(numLayers + 1); layer++)
  {
    const csArray< csArray<float> > &weights = layers[layer].weights;

    data.Push(csBigEndian::Int32((int32) weights.GetSize()));
    data.Push(csBigEndian::Int32((int32) weights[0].GetSize()));

    for (size_t i = 0, endi = weights.GetSize(); i < endi; i++)
    {
      for (size_t j = 0, endj = weights[i].GetSize(); j < endj; j++)
      {
	uint32 portableFloat = csIEEEfloat::FromNative(weights[i][j]);
	data.Push(csBigEndian::Int32(*(int32*) &portableFloat));
      }
    }
  }

  csRef<iDataBuffer> buf (csPtr<iDataBuffer>
    (new CS::DataBuffer<CS::Memory::AllocatorMalloc>
	((char*)data.GetArray(), data.GetSize() * sizeof(int32), false)));

  bool ok = cache->CacheData(buf->GetData(), buf->GetSize(), "pcneuralnet", scope, id);
  if (ok) return true;
  else return Error("Failed to save cache.");
}

bool celPcNeuralNet::LoadCachedWeights(const char *scope, uint32 id)
{
  if (! cache.IsValid()) return Error("No iCacheManager.");

  csRef<iDataBuffer> buf = cache->ReadCache("pcneuralnet", scope, id);
  if (! buf.IsValid()) return Warning("Failed to load cache.");

  size_t idx = 0;
  if (ReadInt32(buf, idx++) != numInputs
   || ReadInt32(buf, idx++) != numOutputs
   || ReadInt32(buf, idx++) != numLayers)
    return Warning("Non-matching size of cache data. Maybe old version?");

  valid = false;
  if (! Validate()) return Warning("Malformed cache data. Maybe old version?");

  for (size_t layer = 0; layer < size_t(numLayers + 1); layer++)
  {
    csArray< csArray<float> > &weights = layers[layer].weights;

    int32 outerSize = ReadInt32(buf, idx++);
    int32 innerSize = ReadInt32(buf, idx++);
    if (weights.GetSize() != size_t(outerSize)
     || weights[0].GetSize() != size_t(innerSize))
	return Warning("Malformed cache data. Maybe old version?");

    for (size_t i = 0, endi = weights.GetSize(); i < endi; i++)
    {
      for (size_t j = 0, endj = weights[i].GetSize(); j < endj; j++)
      {
	int32 portableFloat = ReadInt32(buf, idx++);
	weights[i][j] = csIEEEfloat::ToNative(*(uint32*) &portableFloat);
      }
    }
  }
  return true;
}

int32 celPcNeuralNet::ReadInt32(iDataBuffer *buf, size_t index)
{
  if (index >= (buf->GetSize() / sizeof(int32)))
  {
    Warning("Malformed cache data. Maybe old version?");
    return 0;
  }
  int32 *ptr = (int32*) buf->GetData();
  return csBigEndian::Int32(ptr[index]);
}

bool celPcNeuralNet::Error(char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV(const_cast<iObjectRegistry*>(object_reg),
	CS_REPORTER_SEVERITY_ERROR, "cel.propclass.ai.neuralnet", desc, args);
  va_end(args);
  return false;
}

bool celPcNeuralNet::Warning(char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV(const_cast<iObjectRegistry*>(object_reg),
	CS_REPORTER_SEVERITY_WARNING, "cel.propclass.ai.neuralnet", desc, args);
  va_end(args);
  return false;
}

bool celPcNeuralNet::Bug(char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV(const_cast<iObjectRegistry*>(object_reg),
	CS_REPORTER_SEVERITY_BUG, "cel.propclass.ai.neuralnet", desc, args);
  va_end(args);
  return false;
}
