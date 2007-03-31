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
static char const metainfo_pfmechanics[] =
"<?xml version=\"1.0\"?>"
"<!-- pfmechanics.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.mechsys</name>"
"        <implementation>celPfMechanicsSystem</implementation>"
"        <description>CEL Mechanics System Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.mechobject</name>"
"        <implementation>celPfMechanicsObject</implementation>"
"        <description>CEL Mechanics Object Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.mechjoint</name>"
"        <implementation>celPfMechanicsJoint</implementation>"
"        <description>CEL Mechanics Joint Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.physics.system</name>"
"        <implementation>celPfMechanicsSystem</implementation>"
"        <description>CEL Mechanics System Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.physics.object</name>"
"        <implementation>celPfMechanicsObject</implementation>"
"        <description>CEL Mechanics Object Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.physics.joint</name>"
"        <implementation>celPfMechanicsJoint</implementation>"
"        <description>CEL Mechanics Joint Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfMechanicsSystem_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsSystem_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsSystem) 
  #endif
  #ifndef celPfMechanicsObject_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsObject_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsObject) 
  #endif
  #ifndef celPfMechanicsJoint_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsJoint_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsJoint) 
  #endif
  #ifndef celPfMechanicsSystem_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsSystem_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsSystem) 
  #endif
  #ifndef celPfMechanicsObject_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsObject_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsObject) 
  #endif
  #ifndef celPfMechanicsJoint_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsJoint_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsJoint) 
  #endif

class pfmechanics
{
SCF_REGISTER_STATIC_LIBRARY(pfmechanics,metainfo_pfmechanics)
  #ifndef celPfMechanicsSystem_FACTORY_REGISTERED 
  #define celPfMechanicsSystem_FACTORY_REGISTERED 
    celPfMechanicsSystem_StaticInit celPfMechanicsSystem_static_init__; 
  #endif
  #ifndef celPfMechanicsObject_FACTORY_REGISTERED 
  #define celPfMechanicsObject_FACTORY_REGISTERED 
    celPfMechanicsObject_StaticInit celPfMechanicsObject_static_init__; 
  #endif
  #ifndef celPfMechanicsJoint_FACTORY_REGISTERED 
  #define celPfMechanicsJoint_FACTORY_REGISTERED 
    celPfMechanicsJoint_StaticInit celPfMechanicsJoint_static_init__; 
  #endif
  #ifndef celPfMechanicsSystem_FACTORY_REGISTERED 
  #define celPfMechanicsSystem_FACTORY_REGISTERED 
    celPfMechanicsSystem_StaticInit celPfMechanicsSystem_static_init__; 
  #endif
  #ifndef celPfMechanicsObject_FACTORY_REGISTERED 
  #define celPfMechanicsObject_FACTORY_REGISTERED 
    celPfMechanicsObject_StaticInit celPfMechanicsObject_static_init__; 
  #endif
  #ifndef celPfMechanicsJoint_FACTORY_REGISTERED 
  #define celPfMechanicsJoint_FACTORY_REGISTERED 
    celPfMechanicsJoint_StaticInit celPfMechanicsJoint_static_init__; 
  #endif
public:
 pfmechanics();
};
pfmechanics::pfmechanics() {}

}
