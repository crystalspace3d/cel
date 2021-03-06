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
static char const metainfo_pfinv[] =
"<?xml version=\"1.0\"?>"
"<!-- pfinv.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.inventory</name>"
"        <implementation>celPfInventory</implementation>"
"        <description>CEL Inventory Property Class Factory</description>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.characteristics</name>"
"        <implementation>celPfCharacteristics</implementation>"
"        <description>CEL Characteristics Property Class Factory</description>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.tools.inventory</name>"
"        <implementation>celPfInventory</implementation>"
"        <description>CEL Inventory Property Class Factory</description>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.tools.inventory.characteristics</name>"
"        <implementation>celPfCharacteristics</implementation>"
"        <description>CEL Characteristics Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfInventory_FACTORY_REGISTER_DEFINED 
  #define celPfInventory_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfInventory) 
  #endif
  #ifndef celPfCharacteristics_FACTORY_REGISTER_DEFINED 
  #define celPfCharacteristics_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCharacteristics) 
  #endif
  #ifndef celPfInventory_FACTORY_REGISTER_DEFINED 
  #define celPfInventory_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfInventory) 
  #endif
  #ifndef celPfCharacteristics_FACTORY_REGISTER_DEFINED 
  #define celPfCharacteristics_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCharacteristics) 
  #endif

class pfinv
{
SCF_REGISTER_STATIC_LIBRARY(pfinv,metainfo_pfinv)
  #ifndef celPfInventory_FACTORY_REGISTERED 
  #define celPfInventory_FACTORY_REGISTERED 
    celPfInventory_StaticInit celPfInventory_static_init__; 
  #endif
  #ifndef celPfCharacteristics_FACTORY_REGISTERED 
  #define celPfCharacteristics_FACTORY_REGISTERED 
    celPfCharacteristics_StaticInit celPfCharacteristics_static_init__; 
  #endif
  #ifndef celPfInventory_FACTORY_REGISTERED 
  #define celPfInventory_FACTORY_REGISTERED 
    celPfInventory_StaticInit celPfInventory_static_init__; 
  #endif
  #ifndef celPfCharacteristics_FACTORY_REGISTERED 
  #define celPfCharacteristics_FACTORY_REGISTERED 
    celPfCharacteristics_StaticInit celPfCharacteristics_static_init__; 
  #endif
public:
 pfinv();
};
pfinv::pfinv() {}

}
