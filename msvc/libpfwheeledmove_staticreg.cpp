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
static char const metainfo_pfwheeledmove[] =
"<?xml version=\"1.0\"?>"
"<!-- pfwheeledmove.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.move.actor.wheeled</name>"
"        <implementation>celPfWheeledMove</implementation>"
"        <description>CEL Wheeled Mover Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfWheeledMove_FACTORY_REGISTER_DEFINED 
  #define celPfWheeledMove_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfWheeledMove) 
  #endif

class pfwheeledmove
{
SCF_REGISTER_STATIC_LIBRARY(pfwheeledmove,metainfo_pfwheeledmove)
  #ifndef celPfWheeledMove_FACTORY_REGISTERED 
  #define celPfWheeledMove_FACTORY_REGISTERED 
    celPfWheeledMove_StaticInit celPfWheeledMove_static_init__; 
  #endif
public:
 pfwheeledmove();
};
pfwheeledmove::pfwheeledmove() {}

}
