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
static char const metainfo_stdphyslayer[] =
"<?xml version=\"1.0\"?>"
"<!-- stdphyslayer.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.physicallayer</name>"
"        <implementation>celPlLayer</implementation>"
"	<description>CEL Layer</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPlLayer_FACTORY_REGISTER_DEFINED 
  #define celPlLayer_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPlLayer) 
  #endif

class stdphyslayer
{
SCF_REGISTER_STATIC_LIBRARY(stdphyslayer,metainfo_stdphyslayer)
  #ifndef celPlLayer_FACTORY_REGISTERED 
  #define celPlLayer_FACTORY_REGISTERED 
    celPlLayer_StaticInit celPlLayer_static_init__; 
  #endif
public:
 stdphyslayer();
};
stdphyslayer::stdphyslayer() {}

}
