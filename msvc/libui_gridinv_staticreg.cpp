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
static char const metainfo_ui_gridinv[] =
"<?xml version=\"1.0\"?>"
"<!-- ui_gridinv.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.ui.inventory.grid</name>"
"        <implementation>celUIGridInventory</implementation>"
"	<description>CEL Grid Based Inventory UI</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celUIGridInventory_FACTORY_REGISTER_DEFINED 
  #define celUIGridInventory_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celUIGridInventory) 
  #endif

class ui_gridinv
{
SCF_REGISTER_STATIC_LIBRARY(ui_gridinv,metainfo_ui_gridinv)
  #ifndef celUIGridInventory_FACTORY_REGISTERED 
  #define celUIGridInventory_FACTORY_REGISTERED 
    celUIGridInventory_StaticInit celUIGridInventory_static_init__; 
  #endif
public:
 ui_gridinv();
};
ui_gridinv::ui_gridinv() {}

}
