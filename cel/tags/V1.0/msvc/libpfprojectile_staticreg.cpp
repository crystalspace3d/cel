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
static char const metainfo_pfprojectile[] =
"<?xml version=\"1.0\"?>"
"<!-- pfprojectile.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.pcfactory.projectile</name>"
"        <implementation>celPfProjectile</implementation>"
"	<description>CEL Projectile Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfProjectile_FACTORY_REGISTER_DEFINED 
  #define celPfProjectile_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfProjectile) 
  #endif

class pfprojectile
{
SCF_REGISTER_STATIC_LIBRARY(pfprojectile,metainfo_pfprojectile)
  #ifndef celPfProjectile_FACTORY_REGISTERED 
  #define celPfProjectile_FACTORY_REGISTERED 
    celPfProjectile_StaticInit celPfProjectile_static_init__; 
  #endif
public:
 pfprojectile();
};
pfprojectile::pfprojectile() {}

}
