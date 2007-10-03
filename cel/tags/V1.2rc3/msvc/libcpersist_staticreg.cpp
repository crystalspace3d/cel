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
static char const metainfo_cpersist[] =
"<?xml version=\"1.0\"?>"
"<!-- cpersist.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.persistence.classic</name>"
"        <implementation>celPersistClassic</implementation>"
"	<description>CEL Classic Persistence Module</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPersistClassic_FACTORY_REGISTER_DEFINED 
  #define celPersistClassic_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPersistClassic) 
  #endif

class cpersist
{
SCF_REGISTER_STATIC_LIBRARY(cpersist,metainfo_cpersist)
  #ifndef celPersistClassic_FACTORY_REGISTERED 
  #define celPersistClassic_FACTORY_REGISTERED 
    celPersistClassic_StaticInit celPersistClassic_static_init__; 
  #endif
public:
 cpersist();
};
cpersist::cpersist() {}

}
