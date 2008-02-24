// This file is automatically generated.
#include "cssysdef.h"
#include "csutil/scf.h"

// Put static linking stuff into own section.
// The idea is that this allows the section to be swapped out but not
// swapped in again b/c something else in it was needed.
#if !defined(CS_DEBUG) && defined(CS_COMPILER_MSVC)
#pragma const_seg(".CSmetai")
#pragma comment(linker, "/section:.CSmetai,r")
#pragma code_seg(".CSmeta")
#pragma comment(linker, "/section:.CSmeta,er")
#pragma comment(linker, "/merge:.CSmetai=.CSmeta")
#endif

namespace csStaticPluginInit
{
static char const metainfo_pfneuralnet[] =
"<?xml version=\"1.0\"?>"
"<!-- pfneuralnet.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.ai.neuralnet</name>"
"        <implementation>celPfNeuralNet</implementation>"
"        <description>The neural network property class factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfNeuralNet_FACTORY_REGISTER_DEFINED 
  #define celPfNeuralNet_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfNeuralNet) 
  #endif

class pfneuralnet
{
SCF_REGISTER_STATIC_LIBRARY(pfneuralnet,metainfo_pfneuralnet)
  #ifndef celPfNeuralNet_FACTORY_REGISTERED 
  #define celPfNeuralNet_FACTORY_REGISTERED 
    celPfNeuralNet_StaticInit celPfNeuralNet_static_init__; 
  #endif
public:
 pfneuralnet();
};
pfneuralnet::pfneuralnet() {}

}