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
"      <class>"
"        <name>cel.pcfactory.mechsys</name>"
"        <implementation>celPfMechanicsSystem</implementation>"
"	<description>CEL Mechanics System Class Factory</description>"
"	<requires>"
"	  <class>cel.physicallayer</class>"
"	</requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.mechobject</name>"
"        <implementation>celPfMechanicsObject</implementation>"
"	<description>CEL Mechanics Object Class Factory</description>"
"	<requires>"
"	  <class>cel.physicallayer</class>"
"	</requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.mechjoint</name>"
"        <implementation>celPfMechanicsJoint</implementation>"
"	<description>CEL Mechanics Joint Class Factory</description>"
"	<requires>"
"	  <class>cel.physicallayer</class>"
"	</requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.mechbalancedgroup</name>"
"        <implementation>celPfMechanicsBalancedGroup</implementation>"
"	<description>CEL Mechanics Balanced Thruster Group Class Factory</description>"
"	<requires>"
"	  <class>cel.physicallayer</class>"
"	</requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.mechthrustercontroller</name>"
"        <implementation>celPfMechanicsThrusterController</implementation>"
"	<description>CEL Mechanics Thruster Controller Class Factory</description>"
"	<requires>"
"	  <class>cel.physicallayer</class>"
"	</requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.mechthrustreactionary</name>"
"        <implementation>celPfMechanicsThrusterReactionary</implementation>"
"	<description>CEL Mechanics Reactionary Thruster Class Factory</description>"
"	<requires>"
"	  <class>cel.physicallayer</class>"
"	</requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.hover</name>"
"        <implementation>celPfHover</implementation>"
"        <description>CEL Hover type Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"          <class>cel.mechobject</class>"
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
  #ifndef celPfMechanicsBalancedGroup_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsBalancedGroup_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsBalancedGroup) 
  #endif
  #ifndef celPfMechanicsThrusterController_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsThrusterController_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsThrusterController) 
  #endif
  #ifndef celPfMechanicsThrusterReactionary_FACTORY_REGISTER_DEFINED 
  #define celPfMechanicsThrusterReactionary_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMechanicsThrusterReactionary) 
  #endif
  #ifndef celPfHover_FACTORY_REGISTER_DEFINED 
  #define celPfHover_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfHover) 
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
  #ifndef celPfMechanicsBalancedGroup_FACTORY_REGISTERED 
  #define celPfMechanicsBalancedGroup_FACTORY_REGISTERED 
    celPfMechanicsBalancedGroup_StaticInit celPfMechanicsBalancedGroup_static_init__; 
  #endif
  #ifndef celPfMechanicsThrusterController_FACTORY_REGISTERED 
  #define celPfMechanicsThrusterController_FACTORY_REGISTERED 
    celPfMechanicsThrusterController_StaticInit celPfMechanicsThrusterController_static_init__; 
  #endif
  #ifndef celPfMechanicsThrusterReactionary_FACTORY_REGISTERED 
  #define celPfMechanicsThrusterReactionary_FACTORY_REGISTERED 
    celPfMechanicsThrusterReactionary_StaticInit celPfMechanicsThrusterReactionary_static_init__; 
  #endif
  #ifndef celPfHover_FACTORY_REGISTERED 
  #define celPfHover_FACTORY_REGISTERED 
    celPfHover_StaticInit celPfHover_static_init__; 
  #endif
public:
 pfmechanics();
};
pfmechanics::pfmechanics() {}

}
