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
static char const metainfo_pfdamage[] =
"<?xml version=\"1.0\"?>"
"<!-- pfdamage.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.damage</name>"
"        <implementation>celPfDamage</implementation>"
"	<description>CEL Damage Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfDamage_FACTORY_REGISTER_DEFINED 
  #define celPfDamage_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfDamage) 
  #endif

class pfdamage
{
SCF_REGISTER_STATIC_LIBRARY(pfdamage,metainfo_pfdamage)
  #ifndef celPfDamage_FACTORY_REGISTERED 
  #define celPfDamage_FACTORY_REGISTERED 
    celPfDamage_StaticInit celPfDamage_static_init__; 
  #endif
public:
 pfdamage();
};
pfdamage::pfdamage() {}

}