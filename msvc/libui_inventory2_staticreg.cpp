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
static char const metainfo_ui_inventory2[] =
"<?xml version=\"1.0\"?>"
"<!-- ui_inventory2.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.ui.inventory2</name>"
"        <implementation>celUIInventory2</implementation>"
"	<description>CEL Double Inventory UI</description>"
"        <requires>"
"          <class>crystalspace.cegui.wrapper</class>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celUIInventory2_FACTORY_REGISTER_DEFINED 
  #define celUIInventory2_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celUIInventory2) 
  #endif

class ui_inventory2
{
SCF_REGISTER_STATIC_LIBRARY(ui_inventory2,metainfo_ui_inventory2)
  #ifndef celUIInventory2_FACTORY_REGISTERED 
  #define celUIInventory2_FACTORY_REGISTERED 
    celUIInventory2_StaticInit celUIInventory2_static_init__; 
  #endif
public:
 ui_inventory2();
};
ui_inventory2::ui_inventory2() {}

}
