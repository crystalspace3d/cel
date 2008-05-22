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
static char const metainfo_pfquest[] =
"<?xml version=\"1.0\"?>"
"<!-- pfquest.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.quest</name>"
"        <implementation>celPfQuest</implementation>"
"        <description>CEL Quest Property Class Factory</description>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.logic.quest</name>"
"        <implementation>celPfQuest</implementation>"
"        <description>CEL Quest Property Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfQuest_FACTORY_REGISTER_DEFINED 
  #define celPfQuest_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfQuest) 
  #endif
  #ifndef celPfQuest_FACTORY_REGISTER_DEFINED 
  #define celPfQuest_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfQuest) 
  #endif

class pfquest
{
SCF_REGISTER_STATIC_LIBRARY(pfquest,metainfo_pfquest)
  #ifndef celPfQuest_FACTORY_REGISTERED 
  #define celPfQuest_FACTORY_REGISTERED 
    celPfQuest_StaticInit celPfQuest_static_init__; 
  #endif
  #ifndef celPfQuest_FACTORY_REGISTERED 
  #define celPfQuest_FACTORY_REGISTERED 
    celPfQuest_StaticInit celPfQuest_static_init__; 
  #endif
public:
 pfquest();
};
pfquest::pfquest() {}

}
