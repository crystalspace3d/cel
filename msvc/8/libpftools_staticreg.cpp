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
static char const metainfo_pftools[] =
"<?xml version=\"1.0\"?>"
"<!-- pftools.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.spawn</name>"
"        <implementation>celPfSpawn</implementation>"
"	<description>CEL Spawn Property Class Factory</description>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.tooltip</name>"
"        <implementation>celPfTooltip</implementation>"
"	<description>CEL Tooltip Property Class Factory</description>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.timer</name>"
"        <implementation>celPfTimer</implementation>"
"	<description>CEL Timer Property Class Factory</description>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.properties</name>"
"        <implementation>celPfProperties</implementation>"
"	<description>CEL Properties Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfSpawn_FACTORY_REGISTER_DEFINED 
  #define celPfSpawn_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfSpawn) 
  #endif
  #ifndef celPfTooltip_FACTORY_REGISTER_DEFINED 
  #define celPfTooltip_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfTooltip) 
  #endif
  #ifndef celPfTimer_FACTORY_REGISTER_DEFINED 
  #define celPfTimer_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfTimer) 
  #endif
  #ifndef celPfProperties_FACTORY_REGISTER_DEFINED 
  #define celPfProperties_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfProperties) 
  #endif

class pftools
{
SCF_REGISTER_STATIC_LIBRARY(pftools,metainfo_pftools)
  #ifndef celPfSpawn_FACTORY_REGISTERED 
  #define celPfSpawn_FACTORY_REGISTERED 
    celPfSpawn_StaticInit celPfSpawn_static_init__; 
  #endif
  #ifndef celPfTooltip_FACTORY_REGISTERED 
  #define celPfTooltip_FACTORY_REGISTERED 
    celPfTooltip_StaticInit celPfTooltip_static_init__; 
  #endif
  #ifndef celPfTimer_FACTORY_REGISTERED 
  #define celPfTimer_FACTORY_REGISTERED 
    celPfTimer_StaticInit celPfTimer_static_init__; 
  #endif
  #ifndef celPfProperties_FACTORY_REGISTERED 
  #define celPfProperties_FACTORY_REGISTERED 
    celPfProperties_StaticInit celPfProperties_static_init__; 
  #endif
public:
 pftools();
};
pftools::pftools() {}

}
