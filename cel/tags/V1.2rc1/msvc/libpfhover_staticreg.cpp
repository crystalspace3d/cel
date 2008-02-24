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
static char const metainfo_pfhover[] =
"<?xml version=\"1.0\"?>"
"<!-- pfhover.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.hover</name>"
"        <implementation>celPfHover</implementation>"
"        <description>CEL Hover type Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"          <class>cel.mechobject</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.craft</name>"
"        <implementation>celPfCraftController</implementation>"
"        <description>CEL Craft Controller Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"          <class>cel.mechobject</class>"
"        </requires>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.vehicle.hover</name>"
"        <implementation>celPfHover</implementation>"
"        <description>CEL Hover type Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"          <class>cel.mechobject</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.vehicle.craft</name>"
"        <implementation>celPfCraftController</implementation>"
"        <description>CEL Craft Controller Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"          <class>cel.mechobject</class>"
"        </requires>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfHover_FACTORY_REGISTER_DEFINED 
  #define celPfHover_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfHover) 
  #endif
  #ifndef celPfCraftController_FACTORY_REGISTER_DEFINED 
  #define celPfCraftController_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCraftController) 
  #endif
  #ifndef celPfHover_FACTORY_REGISTER_DEFINED 
  #define celPfHover_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfHover) 
  #endif
  #ifndef celPfCraftController_FACTORY_REGISTER_DEFINED 
  #define celPfCraftController_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCraftController) 
  #endif

class pfhover
{
SCF_REGISTER_STATIC_LIBRARY(pfhover,metainfo_pfhover)
  #ifndef celPfHover_FACTORY_REGISTERED 
  #define celPfHover_FACTORY_REGISTERED 
    celPfHover_StaticInit celPfHover_static_init__; 
  #endif
  #ifndef celPfCraftController_FACTORY_REGISTERED 
  #define celPfCraftController_FACTORY_REGISTERED 
    celPfCraftController_StaticInit celPfCraftController_static_init__; 
  #endif
  #ifndef celPfHover_FACTORY_REGISTERED 
  #define celPfHover_FACTORY_REGISTERED 
    celPfHover_StaticInit celPfHover_static_init__; 
  #endif
  #ifndef celPfCraftController_FACTORY_REGISTERED 
  #define celPfCraftController_FACTORY_REGISTERED 
    celPfCraftController_StaticInit celPfCraftController_static_init__; 
  #endif
public:
 pfhover();
};
pfhover::pfhover() {}

}