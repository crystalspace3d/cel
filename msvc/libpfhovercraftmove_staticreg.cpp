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
static char const metainfo_pfhovercraftmove[] =
"<?xml version=\"1.0\"?>"
"<!-- pfhovercraftmove.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.move.actor.hovercraft</name>"
"        <implementation>celPfHoverCraftMove</implementation>"
"        <description>CEL HoverCraft Mover Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfHoverCraftMove_FACTORY_REGISTER_DEFINED 
  #define celPfHoverCraftMove_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfHoverCraftMove) 
  #endif

class pfhovercraftmove
{
SCF_REGISTER_STATIC_LIBRARY(pfhovercraftmove,metainfo_pfhovercraftmove)
  #ifndef celPfHoverCraftMove_FACTORY_REGISTERED 
  #define celPfHoverCraftMove_FACTORY_REGISTERED 
    celPfHoverCraftMove_StaticInit celPfHoverCraftMove_static_init__; 
  #endif
public:
 pfhovercraftmove();
};
pfhovercraftmove::pfhovercraftmove() {}

}
