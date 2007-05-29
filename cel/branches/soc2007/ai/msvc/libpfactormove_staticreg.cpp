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
static char const metainfo_pfactormove[] =
"<?xml version=\"1.0\"?>"
"<!-- pfactormove.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.actormove</name>"
"        <implementation>celPfActorMove</implementation>"
"        <description>CEL Actor Movement Class Factory</description>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.npcmove</name>"
"        <implementation>celPfNpcMove</implementation>"
"        <description>CEL NPC Movement Class Factory</description>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.move.actorold</name>"
"        <implementation>celPfActorMove</implementation>"
"        <description>CEL Actor Movement Class Factory</description>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.move.npc</name>"
"        <implementation>celPfNpcMove</implementation>"
"        <description>CEL NPC Movement Class Factory</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfActorMove_FACTORY_REGISTER_DEFINED 
  #define celPfActorMove_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfActorMove) 
  #endif
  #ifndef celPfNpcMove_FACTORY_REGISTER_DEFINED 
  #define celPfNpcMove_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfNpcMove) 
  #endif
  #ifndef celPfActorMove_FACTORY_REGISTER_DEFINED 
  #define celPfActorMove_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfActorMove) 
  #endif
  #ifndef celPfNpcMove_FACTORY_REGISTER_DEFINED 
  #define celPfNpcMove_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfNpcMove) 
  #endif

class pfactormove
{
SCF_REGISTER_STATIC_LIBRARY(pfactormove,metainfo_pfactormove)
  #ifndef celPfActorMove_FACTORY_REGISTERED 
  #define celPfActorMove_FACTORY_REGISTERED 
    celPfActorMove_StaticInit celPfActorMove_static_init__; 
  #endif
  #ifndef celPfNpcMove_FACTORY_REGISTERED 
  #define celPfNpcMove_FACTORY_REGISTERED 
    celPfNpcMove_StaticInit celPfNpcMove_static_init__; 
  #endif
  #ifndef celPfActorMove_FACTORY_REGISTERED 
  #define celPfActorMove_FACTORY_REGISTERED 
    celPfActorMove_StaticInit celPfActorMove_static_init__; 
  #endif
  #ifndef celPfNpcMove_FACTORY_REGISTERED 
  #define celPfNpcMove_FACTORY_REGISTERED 
    celPfNpcMove_StaticInit celPfNpcMove_static_init__; 
  #endif
public:
 pfactormove();
};
pfactormove::pfactormove() {}

}
